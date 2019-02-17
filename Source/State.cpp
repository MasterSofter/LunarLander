#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
	MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys1)
	: window(&window),
	textures(&textures),
	fonts(&fonts),
	music(&music),
	sounds(&sounds),
	keys(&keys1)
{
}

State::State(StateStack& stack, Context context)
	: _stackPtr(&stack),
	_context(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	_stackPtr->pushState(stateID);
}

void State::requestStackPop()
{
	_stackPtr->popState();
}

void State::requestStateClear()
{
	_stackPtr->clearStates();
}

State::Context State::getContext() const
{
	return _context;
}

void State::onActivate()
{

}

void State::onDestroy()
{

}
