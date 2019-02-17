#ifndef SPRITENODE_H_
#define SPRITENODE_H_

#include "SceneNode.h"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
	public:
		explicit SpriteNode(const sf::Texture& texture);
		SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, Category::Type category = Category::None);

		sf::Sprite& getSprite() { return _sprite; }

private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Sprite _sprite;
};

#endif // SPRITENODE_H_
