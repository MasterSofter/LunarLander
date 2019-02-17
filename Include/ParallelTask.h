#ifndef LUNARLANDER_PARALLELTASK_H
#define LUNARLANDER_PARALLELTASK_H

#include "State.h"
#include "ResourceIdentifiers.h"
#include <vector>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>


class State;

class ParallelTask
{
public:
    ParallelTask(State::Context context);
    void execute();
    bool isFinished();

private:
    void				runTask();

private:
    std::vector<std::pair<Textures::ID, sf::String>> _initTextures;
    State::Context _context;
    sf::Thread	   _thread;
    bool		   _finished;
    sf::Mutex	   _mutex;
};


#endif //LUNARLANDER_PARALLELTASK_H
