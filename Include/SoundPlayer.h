#ifndef SOUNDPLAYER_H_
#define SOUNDPLAYER_H_

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
	public:
		SoundPlayer();

		void play(SoundEffect::ID effect);
		void play(SoundEffect::ID effect, sf::Vector2f position);

		void removeStoppedSounds();
		void setListenerPosition(sf::Vector2f position);
		sf::Vector2f getListenerPosition() const;


	private:
		SoundBufferHolder _soundBuffers;
		std::list<sf::Sound> _sounds;
};

#endif // SOUNDPLAYER_H_
