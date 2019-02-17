#ifndef TITLESTATE_H_
#define TITLESTATE_H_

#include "State.h"
#include <vector>
#include "ParallelTask.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class StateTitle : public State
{
	public:
		StateTitle(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Sprite backgroundSprite;
		sf::Time textEffectTime;
        ParallelTask _loadTask;
};

#endif // TITLESTATE_H_
