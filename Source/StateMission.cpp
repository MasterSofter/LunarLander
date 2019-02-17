#include "StateMission.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>


StateMission::StateMission(StateStack& stack, Context context)
        : State(stack, context),
          text(),
          showText(true),
          textEffectTime(sf::Time::Zero)
{
    backgroundSprite.setTexture(context.textures->get(Textures::MissionScreen));

    sf::FloatRect bounds = backgroundSprite.getLocalBounds();
    backgroundSprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    backgroundSprite.setPosition(context.window->getSize().x/2.f, context.window->getSize().y/2.f);
    backgroundSprite.setScale(1.25, 1.2);

    text.setFont(context.fonts->get(Fonts::Main));
    text.setString(L"Нажмите любую кнопку");
    text.setFillColor(sf::Color::Black);

    bounds = text.getLocalBounds();
    text.setPosition(context.window->getSize().x - bounds.width - bounds.height - 15, context.window->getSize().y - bounds.height - bounds.height/2 - 3);
}

void StateMission::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(backgroundSprite);

    if (showText)
        window.draw(text);
}

bool StateMission::update(sf::Time dt)
{
    textEffectTime += dt;

    if (textEffectTime >= sf::seconds(0.5f))
    {
        showText = !showText;
        textEffectTime = sf::Time::Zero;
    }

    return true;
}

bool StateMission::handleEvent(const sf::Event& event)
{
    // If any key is pressed, trigger the next screen
    if (event.type == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush(States::Game);
    }

    return true;
}