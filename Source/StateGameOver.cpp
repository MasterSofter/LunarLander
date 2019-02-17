#include "StateGameOver.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


StateGameOver::StateGameOver(StateStack& stack, Context context, const std::string& text)
: State(stack, context)
, _gameOverText()
, _elapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());
	
	_gameOverText.setFont(font);
	_gameOverText.setString(text);
	_gameOverText.setCharacterSize(70);
	centerOrigin(_gameOverText);
	_gameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void StateGameOver::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 50));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(_gameOverText);
}

bool StateGameOver::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	_elapsedTime += dt;

	if (_elapsedTime > sf::seconds(5))
	{
		requestStateClear();
		requestStackPush(States::Mission);
	}

	return false;
}

bool StateGameOver::handleEvent(const sf::Event& event)
{
    /*
	if (event.type == sf::Event::KeyReleased)
	{
		requestStateClear();
		requestStackPush(States::Mission);
	}
	return true;
*/
    return false;
}
