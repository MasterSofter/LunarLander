#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "State.h"
#include "World.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class StateGame : public State
{
	public:
		StateGame(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		World	_world;
		Player	_player;
};

#endif // GAMESTATE_H_