#ifndef LUNARLANDER_MODULE_H
#define LUNARLANDER_MODULE_H

#include "SceneNode.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "Animation.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <math.h>

class World;

class LunarModuleNode : public SceneNode
{
public:
    typedef std::unique_ptr<LunarModuleNode> Ptr;

private:
    World& _world;
    sf::Sprite _moduleSprite;
    sf::Sprite _leftSprite;
    sf::Sprite _rightSprite;
    sf::Sprite _crashSprite;
    Animation _flame;

    // координаты модуля в метрах
    // x - направо
    // y - вверх
    sf::Vector2f _pos;
    sf::Vector2f _init_pos;

    Vector2d _velocity; // meters/second

    typedef struct VelAccel {
        Vector2d vel;
        Vector2d accel;
    } VelAccel;

    sf::String _name;

    // Тяга РСУ, Ньютон
    float _rsu_thrust;

    double _orientation; // radians from x axis
    double _spin_rate;   // radians/second. positive is cw, negative is ccw

    // параметры во время касания земли
    double _landingOrientation;
    double _landingSpinRate;

    double _fuel;        // kg
    double _dry_mass;    // kg (const)

    // начальное кол-во топлива
    double _init_fuel;   // kg (const)

    // текущая тяга
    double _thrust;      // Newtons

    // полная тяга
    double _max_thrust;  // Newtons (const)

    // расход топлива на полной тяге
    double _fuel_rate;   // kg/s

    // угловое ускорение при повороте модуля вокруг центра масс
    double _torque = 0.;      // radians/s^2.
    constexpr static const double SAFE_VEL = 15.; // pixels/s

    bool _landed;
    bool _landedSafe;

    // максиамальная вертикальная скорость безопасного прилунения, м/с
    double _safe_landing_vert_vel = 3.;  // m/s

    // максимальная горизонтальная скорость безопасного прилунения, м/с
    double _safe_landing_horz_vel = 2.f; // m/s

    //максимальный угол отклонения от вертикали для безопасного прилунения, рад.
    double _safe_landing_angle = M_PI_2 / 15.f;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);

    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

    VelAccel next_vel_accel(double dt, bool real);

    double fuel_remaining() const;

public:
    LunarModuleNode(World *world);
    void LoadConfig(const std::string& fileName);

    double maxTorque() const;
    void setTorque(int value);

    bool hasSafeLanded() const;
    bool hasLanded() const;

    double maxThrust() const { return _max_thrust; }
    double getThrust() const { return _thrust; }
    float getFuel() const { return _fuel; }
    float getInitFuel() const { return _init_fuel; }

    Vector2d getVelocity() const { return _velocity; }
    sf::Vector2f getPos() const { return _pos; }
    sf::Vector2f getInitPos() const { return _init_pos; }
    void setThrust(double value);
};

#endif //LUNARLANDER_MODULE_H
