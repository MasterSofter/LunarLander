#ifndef GAMEOVERSTATE_H_
#define GAMEOVERSTATE_H_

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class StateGameOver : public State
{
	public:
		StateGameOver(StateStack& stack, Context context, const std::string& text);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text _gameOverText;
		sf::Time _elapsedTime;
};

#endif // GAMEOVERSTATE_H_
