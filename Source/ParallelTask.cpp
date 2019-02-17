#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "ParallelTask.h"

#include <SFML/Graphics/Texture.hpp>

ParallelTask::ParallelTask(State::Context context)
        : _thread(&ParallelTask::runTask, this),
        _context(context),
        _finished(false)
{
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::MissionScreen, "Media/Textures/MissionScreen.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::LunarModule, "Media/Textures/LunarModule.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::LunarModule_left, "Media/Textures/LunarModule_left.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::LunarModule_right, "Media/Textures/LunarModule_right.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::LunarModule_crash, "Media/Textures/LunarModule_crash.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::Flame, "Media/Textures/flame.png"));
    //_initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::Flame, "Media/Textures/rocket_flame.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::Earth, "Media/Textures/semi_earth.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::Moon, "Media/Textures/moon.png"));
    _initTextures.push_back(std::pair<Textures::ID, sf::String>(Textures::Buttons, "Media/Textures/Buttons.png"));
}

void ParallelTask::execute()
{
    _finished = false;
    _thread.launch();
}

bool ParallelTask::isFinished()
{
    sf::Lock lock(_mutex);
    return _finished;
}

void ParallelTask::runTask()
{
    // Dummy task - stall 10 seconds
    while (_initTextures.size() > 0)
    {
        _context.textures->load(_initTextures.back().first, _initTextures.back().second);
        _initTextures.pop_back();
    }

    sf::Lock lock(_mutex);
    _finished = true;
}