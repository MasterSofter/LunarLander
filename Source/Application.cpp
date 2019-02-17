#include <StateMission.h>
#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "StateTitle.h"
#include "StateGame.h"
#include "StatePause.h"
#include "StateGameOver.h"


const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
	: _window(sf::VideoMode(1600, 1024), L"Lunar landing", sf::Style::Close),
	_textures(),
	_fonts(),
	_music(),
	_sounds(),
	_keyBinding(),
	_stateStack(State::Context(_window, _textures, _fonts, _music, _sounds, _keyBinding)),
	_statisticsText(),
	_statisticsUpdateTime(),
	_statisticsNumFrames(0)
{
	_window.setKeyRepeatEnabled(false);
	_window.setVerticalSyncEnabled(true);

	_fonts.load(Fonts::Digital, "Media/Fonts/Open-Display-St.ttf");
    _fonts.load(Fonts::Main, "Media/Fonts/Jura-Regular.ttf");

	_textures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");

	_statisticsText.setFont(_fonts.get(Fonts::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10u);

	registerStates();
	_stateStack.pushState(States::Title);

	_music.setVolume(25.f);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (_stateStack.isEmpty())
				_window.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		_stateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			_window.close();
	}
}

void Application::update(sf::Time dt)
{
	_stateStack.update(dt);
}

void Application::render()
{
	_window.clear();

	_stateStack.draw();

	_window.setView(_window.getDefaultView());
	_window.draw(_statisticsText);

	_window.display();
}

void Application::updateStatistics(sf::Time dt)
{
	_statisticsUpdateTime += dt;
	_statisticsNumFrames += 1;
	if (_statisticsUpdateTime >= sf::seconds(1.0f))
	{
		_statisticsText.setString("FPS: " + toString(_statisticsNumFrames));
		_statisticsUpdateTime -= sf::seconds(1.0f);
		_statisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	_stateStack.registerState<StateTitle>(States::Title);
    _stateStack.registerState<StateMission>(States::Mission);
	_stateStack.registerState<StateGame>(States::Game);
	_stateStack.registerState<StatePause>(States::Pause);
	_stateStack.registerState<StateGameOver>(States::GameOver, "Mission Failed!");
	_stateStack.registerState<StateGameOver>(States::MissionSuccess, "Mission Successful!");
}
