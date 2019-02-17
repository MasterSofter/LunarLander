#include "TextNode.h"
#include "Utility.h"
#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const sf::String& string, const sf::Font& font, unsigned int characterSize)
	: _alignment(Alignment::Center)
{
	_text.setFont(font);
	_text.setCharacterSize(characterSize);
	setString(string);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_text, states);
}

void TextNode::setString(const std::string& text)
{
	_text.setString(text);

	if(_alignment == Alignment::Center)
	{
		centerOrigin(_text);
	}
	else if (_alignment == Alignment::Left)
	{
		sf::FloatRect bounds = _text.getLocalBounds();
		_text.setOrigin(0.f, std::floor(bounds.top + bounds.height / 2.f));
	}
	else
	{
		sf::FloatRect bounds = _text.getLocalBounds();
		_text.setOrigin(bounds.width, std::floor(bounds.top + bounds.height / 2.f));
	}
}

void TextNode::setColor(sf::Color color)
{
	_text.setFillColor(color);
}

sf::FloatRect TextNode::getBoundingRect() const
{
	return getWorldTransform().transformRect(_text.getGlobalBounds());
}


