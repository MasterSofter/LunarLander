#include "LunarModuleNode.h"
#include "Utility.h"
#include "World.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <LunarModuleNode.h>
#include <iostream>
#include <cmath>
#include <fstream>

#ifdef WIN32
    #include <Shlwapi.h>
#else
    #include <unistd.h>
#endif


LunarModuleNode::LunarModuleNode(World* world)
    :SceneNode(Category::Type::SceneLunarModule),
     _pos(0, 2300),
     _init_pos(0, 2300),
     _velocity(152.78, -45.f),
     _world(*world),
     _orientation(0.f),
     _landingOrientation(0.f),
     _dry_mass(1725 + 4627),
     _init_fuel(3000),
     _fuel(3000),
     _spin_rate(0.f),
     _landingSpinRate(0.f),
     _thrust(0.f),
     _max_thrust(4760.f),
     _fuel_rate(14.9f),
     _landed(false),
     _landedSafe(false),
     _moduleSprite(world->getTextures().get(Textures::LunarModule)),
     _leftSprite(world->getTextures().get(Textures::LunarModule_left)),
     _rightSprite(world->getTextures().get(Textures::LunarModule_right)),
     _crashSprite(world->getTextures().get(Textures::LunarModule_crash)),
     _flame(world->getTextures().get(Textures::Flame))
{
    auto pt = sf::Vector2f(_world.getSize() / 2u);
    pt.y -= 100.f;
    setPosition(pt);
    //setScale(0.5f, 0.5f);

    sf::FloatRect bounds = getBoundingRect();
    setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

    _moduleSprite.setScale(0.5f, 0.5f);
    _leftSprite.setScale(0.5f, 0.5f);
    _rightSprite.setScale(0.5f, 0.5f);
    _crashSprite.setScale(0.5f, 0.5f);

    centerOrigin(_moduleSprite);
    centerOrigin(_leftSprite);
    centerOrigin(_rightSprite);

    bounds = _crashSprite.getLocalBounds();
    _crashSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), -bounds.height);

    _flame.setFrameSize(sf::Vector2i(346, 193));
    _flame.setDuration(sf::seconds(0.032));
    _flame.setNumFrames(3);
    _flame.setRepeating(true);

    _flame.setOrigin(_flame.getFrameSize().x, _flame.getFrameSize().y/2.f);
}

void LunarModuleNode::LoadConfig(const std::string& fileName)
{
    std::ifstream file;
    std::string configName;

#ifdef WIN32
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    configName = std::string(buffer).substr(0, pos+1) + fileName;
#else
    char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) != nullptr)
		configName = std::string(cwd) + std::string("/") + fileName;
#endif

    //std::cout << configName;
    file.open(configName);

    if (!file.is_open()){
        std::cerr << "Failed to load file: " << fileName << std::endl;
        return;
    }

    std::string line;

    while(std::getline(file, line))
    {
        std::string name;
        std::stringstream params(line);
        params >> name;

        if(name.length() < 2) continue;
        if(name[0] == '/' && name[1] == '/') continue;

        if (name == "name") {
            std::string value;
            params >> value;
            _name = value;
        }
        else if (name == "init_h")
            params >> _init_pos.y;
        else if (name == "init_horz_vel")
            params >> _velocity.x;
        else if (name == "init_vert_vel")
            params >> _velocity.y;
        else if (name == "dry_mass")
            params >> _dry_mass;
        else if (name == "main_thrust")
            params >> _max_thrust;
        else if (name == "fuel")
            params >> _init_fuel;
        else if (name == "fuel_rate")
            params >> _fuel_rate;
        else if (name == "rsu_thrust")
            params >> _rsu_thrust;
        else if (name == "orientation") {
            double value; params >> value;
            _orientation = toRadian(value);
        }
        else if (name == "save_landing_vert_vel")
            params >> _safe_landing_vert_vel;
        else if (name == "save_landing_horz_vel")
            params >> _safe_landing_horz_vel;
        else if (name == "save_landing_angle") {
            float value; params >> value;
            _safe_landing_angle = toRadian(value);
        }
    }

    _fuel = _init_fuel;
    _pos = _init_pos;

    file.close();
}

double LunarModuleNode::maxTorque() const
{
    return _rsu_thrust/((_dry_mass + _fuel)*2.135);
}

bool LunarModuleNode::hasSafeLanded() const
{
    return _landed &&
            abs(_velocity.y) <= _safe_landing_vert_vel &&
            abs(_velocity.x) <= _safe_landing_horz_vel &&
            abs(M_PI_2 - _landingOrientation) <= _safe_landing_angle &&
            abs(_landingSpinRate) <= 0.05f;
}

bool LunarModuleNode::hasLanded() const
{
    return _landed;
}

void LunarModuleNode::setThrust(double value)
{
    _thrust  = value;

    if(_thrust > _max_thrust)
        _thrust = _max_thrust;
    else if(_thrust < 0.f)
        _thrust = 0.f;
}

void LunarModuleNode::setTorque(int value)
{
    _torque = value * maxTorque();
}

void LunarModuleNode::updateCurrent(sf::Time tm, CommandQueue& commands)
{
    const double dt = tm.asSeconds();

    if(!_landed) {
        // rotation
        _spin_rate += _torque * dt;
        _orientation += _spin_rate * dt;

        if (_orientation > 2 * M_PI) {
            _orientation -= 2 * M_PI;
        } else if (_orientation < 0) {
            _orientation += 2 * M_PI;
        }

        auto va = next_vel_accel(dt, true);

        _velocity = va.vel;

        _pos.x += (_velocity.x * dt + .5 * va.accel.x * dt * dt);
        _pos.y += (_velocity.y * dt + .5 * va.accel.y * dt * dt);

        if(_pos.y <= 0.f)
        {
            _landed = true;

            _landingSpinRate = _spin_rate;
            _landingOrientation = _orientation;

            std::cout << "_velocity.x=" << _velocity.x  << " _velocity.y=" << _velocity.y
                << " _landingOrientation=" << toDegree(_landingOrientation)
                << "_landingSpinRate=" << _landingSpinRate << std::endl;

            _landedSafe = hasSafeLanded();

            _orientation = M_PI_2;
            _spin_rate = 0.f;
            _thrust = 0.f;
            _torque = 0.f;
            _pos.y = 0.f;
        }
    }

    // на экране углы против час. стрелки от оси х экрана
    _moduleSprite.setRotation(-toDegree(_orientation));
    _leftSprite.setRotation(-toDegree(_orientation));
    _rightSprite.setRotation(-toDegree(_orientation));

    if(_thrust > 0.f)
    {
        // включить двигатель
        _flame.update(tm);
        _flame.setRotation(-toDegree(_orientation));
        _flame.setScale(0.4 + _thrust*0.6/_max_thrust, 0.3 + _thrust*0.7/_max_thrust);
    }
}

void LunarModuleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(_thrust > 0.f)
    {
        // включить двигатель
        target.draw(_flame, states);
    }

    if(!hasLanded()) {
        if (_torque > 0.f)
            target.draw(_leftSprite, states);
        else if (_torque < 0.f)
            target.draw(_rightSprite, states);
        else
            target.draw(_moduleSprite, states);
    }
    else
    {
        if(hasSafeLanded())
            target.draw(_moduleSprite, states);
        else
            target.draw(_crashSprite, states);
    }
}


double LunarModuleNode::fuel_remaining() const
{
    return _fuel / _init_fuel;
}

LunarModuleNode::VelAccel LunarModuleNode::next_vel_accel(double dt, bool real)
{
    VelAccel va;
    va.accel.x = 0.;
    va.accel.y = 0.;

    if (_thrust > 0.)
    {
        // compute acceleration
        va.accel.x = _thrust * cos(_orientation) / (_dry_mass + _fuel);
        va.accel.y = _thrust * sin(_orientation) / (_dry_mass + _fuel);

        //printf("_thrust = %f\n", _thrust);

        if (real) {
            // расчет изменения массы
            _fuel -= _thrust * _fuel_rate / _max_thrust * dt;

            if (_fuel <= 0.f)
            {
                _fuel = 0.f;
                _thrust = 0.f;
            }
        }
    }

    va.accel.y += World::g;

    va.vel.x = _velocity.x + va.accel.x * dt;
    va.vel.y = _velocity.y + va.accel.y * dt;
    return va;
}

