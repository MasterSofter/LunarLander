#include "StarfieldNode.h"

StarfieldNode::StarfieldNode(sf::Vector2f size) :
    SceneNode(),
    _starfield(size, 3000, sf::Color::White)
{
}
/*
void StarfieldNode::updateCurrent(sf::Time dt, CommandQueue&)
{
    const float speed = 200.f; // speed of movement
    _starfield.move(sf::Vector2f(0.f, -speed * dt.asSeconds()));
}
*/
void StarfieldNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_starfield);
}

void StarfieldNode::move(Vector2d velocity, sf::Time dt)
{
    _starfield.move(sf::Vector2f(-(float)(velocity.x * 2.f * dt.asSeconds()), (float)(velocity.y * 2.f * dt.asSeconds())));
}