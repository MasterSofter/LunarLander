#include "StateTitle.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


StateTitle::StateTitle(StateStack& stack, Context context)
	: State(stack, context),
	textEffectTime(sf::Time::Zero),
    _loadTask(context)
{
	backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
    _loadTask.execute();
}

void StateTitle::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(backgroundSprite);
}

bool StateTitle::update(sf::Time dt)
{
	textEffectTime += dt;

	if (textEffectTime >= sf::seconds(0.5f) && _loadTask.isFinished())
	{
		requestStackPop();
		requestStackPush(States::Mission);
	}

	return true;
}

bool StateTitle::handleEvent(const sf::Event& event)
{
	return true;
}
