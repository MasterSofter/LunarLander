#ifndef LUNARLANDER_STATEMISSION_H
#define LUNARLANDER_STATEMISSION_H


#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class StateMission : public State
{
public:
    StateMission(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);


private:
    sf::Sprite backgroundSprite;
    sf::Text text;

    bool showText;
    sf::Time textEffectTime;
};

#endif //LUNARLANDER_STATEMISSION_H
