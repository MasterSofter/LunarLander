#ifndef TEXTNODE_H_
#define TEXTNODE_H_

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>


class TextNode : public SceneNode
{
	public:
	enum Alignment
	{
		Left,
		Right,
		Center
	};

	public:
		explicit TextNode(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30);
		void setString(const std::string& text);
		void setColor(sf::Color color);
		virtual sf::FloatRect getBoundingRect() const;
		sf::Text& getText() { return _text; }
		void setAlignment(Alignment alignment) {_alignment = alignment;}

	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text _text;
		Alignment _alignment;
};

#endif // TEXTNODE_H_
