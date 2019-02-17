#ifndef PAUSESTATE_H_
#define PAUSESTATE_H_

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class StatePause : public State
{
	public:
		StatePause(StateStack& stack, Context context, bool letUpdatesThrough = false);
	   ~StatePause();

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Sprite _backgroundSprite;
		sf::Text _pausedText;
		GUI::Container _guiContainer;
		bool _letUpdatesThrough;
};

#endif // PAUSESTATE_H_