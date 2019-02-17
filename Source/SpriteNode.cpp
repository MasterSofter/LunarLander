#include "SpriteNode.h"

#include <SFML/Graphics/RenderTarget.hpp>


SpriteNode::SpriteNode(const sf::Texture& texture)
: _sprite(texture)
{
}	

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, Category::Type category)
    : SceneNode(category), _sprite(texture, textureRect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}