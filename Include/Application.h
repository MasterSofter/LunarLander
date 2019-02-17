#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "KeyBinding.h"
#include "StateStack.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
	public:
		Application();
		void run();

	private:
		void processInput();
		void update(sf::Time dt);
		void render();

		void updateStatistics(sf::Time dt);
		void registerStates();

	private:
		static const sf::Time TimePerFrame;

		sf::RenderWindow _window;
		TextureHolder _textures;
	  	FontHolder _fonts;
		MusicPlayer _music;
		SoundPlayer	_sounds;

		KeyBinding	_keyBinding;
		StateStack	_stateStack;

		sf::Text	_statisticsText;
		sf::Time	_statisticsUpdateTime;
		std::size_t	_statisticsNumFrames;
};

#endif // "APPLICATION_H_
