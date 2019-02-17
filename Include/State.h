#ifndef STATE_H_
#define STATE_H_

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>


namespace sf
{
	class RenderWindow;
}

class StateStack;
class MusicPlayer;
class SoundPlayer;
class KeyBinding;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
					MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys);

			sf::RenderWindow*	window;
			TextureHolder*		textures;
			FontHolder*			fonts;
			MusicPlayer*		music;
			SoundPlayer*		sounds;
			KeyBinding*			keys;
		};

	public:
		State(StateStack& stack, Context context);
		virtual	~State();

		virtual void draw() = 0;
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& event) = 0;

		virtual void onActivate();
		virtual void onDestroy();

	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		Context getContext() const;

	private:
		StateStack* _stackPtr;
		Context _context;
};

#endif // STATE_H_
