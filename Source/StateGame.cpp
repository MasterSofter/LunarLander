#include "StateGame.h"
#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>


StateGame::StateGame(StateStack& stack, Context context)
    : State(stack, context),
	_world(context),
	_player(context.keys)
{
//	_world.addLunarModule();
	_player.setMissionStatus(Player::MissionRunning);

	// Play game theme
	//context.music->play(Music::MissionTheme);
}

void StateGame::draw()
{
	_world.draw();
}

bool StateGame::update(sf::Time dt)
{
	_world.update(dt);

	if (_world.hasLanded())
	{
		if(_world.hasSafeLanded())
			requestStackPush(States::MissionSuccess);
		else
			requestStackPush(States::GameOver);
	}

	CommandQueue& commands = _world.getCommandQueue();
	_player.handleRealtimeInput(commands);

	return true;
}

bool StateGame::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = _world.getCommandQueue();
	_player.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}