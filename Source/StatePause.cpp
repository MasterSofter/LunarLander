#include "StatePause.h"
#include "Button.h"
#include "Utility.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


StatePause::StatePause(StateStack& stack, Context context, bool letUpdatesThrough)
: State(stack, context),
	_backgroundSprite(),
	_pausedText(),
	_guiContainer(),
	_letUpdatesThrough(letUpdatesThrough)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	_pausedText.setFont(font);
	_pausedText.setString("Game Paused");
	_pausedText.setCharacterSize(70);
	centerOrigin(_pausedText);
	_pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto returnButton = std::make_shared<GUI::Button>(context);
	returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	returnButton->setText("Continue");
	returnButton->setCallback([this] ()
	{
		requestStackPop();
	});

	auto backToMenuButton = std::make_shared<GUI::Button>(context);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("New Game");
	backToMenuButton->setCallback([this] ()
	{
		requestStateClear();
		requestStackPush(States::Mission);
	});

    auto exitButton = std::make_shared<GUI::Button>(context);
    exitButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
    exitButton->setText("Exit");
    exitButton->setCallback([this] ()
    {
        requestStateClear();
    });

    _guiContainer.pack(returnButton);
	_guiContainer.pack(backToMenuButton);
    _guiContainer.pack(exitButton);

	getContext().music->setPaused(true);
}

StatePause::~StatePause()
{
	getContext().music->setPaused(false);
}

void StatePause::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(_pausedText);
	window.draw(_guiContainer);
}

bool StatePause::update(sf::Time)
{
	return _letUpdatesThrough;
}

bool StatePause::handleEvent(const sf::Event& event)
{
	_guiContainer.handleEvent(event);
	return false;
}
