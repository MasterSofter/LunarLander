#include "Utility.h"
#include "ProgressBarNode.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <TextNode.h>
#include <SceneNode.h>


ProgressBarNode::ProgressBarNode(World& world, float maxValue, sf::Vector2f size, float angle, bool labelLeft) :
    _world(world),
    _padding(20.f, 0.f),
    _maxValue(maxValue),
    _progressColor(5, 98, 1, 255),
    _labelPersent(false)
{
    _progressBar.setShowBackgroundAndFrame(true);
    _progressBar.setColor(_progressColor);
    _progressBar.setFrameColor(sf::Color::White);
    _progressBar.setSize(size);
    _progressBar.rotate(angle);

    std::unique_ptr<TextNode> text(new TextNode("", world.getFonts().get(Fonts::Main), world.TEXT_HEIGHT));

    text->setPosition(size.y + 20, 0.f);
    text->setAlignment(TextNode::Alignment::Left);

    text->setColor(_progressColor);
    text->setAlignment(TextNode::Alignment::Left);
    _valueLabel = text.get();

    attachChild(std::move(text));
}

void ProgressBarNode::setLabelSide(TextNode::Alignment alignment)
{
    if(alignment == TextNode::Alignment::Left)
    {
        _valueLabel->setPosition(-20, 0.f);
        _valueLabel->setAlignment(TextNode::Alignment::Right);
    }
    else
    {
        _valueLabel->setPosition(_progressBar.getSize().y + 20, 0.f);
        _valueLabel->setAlignment(TextNode::Alignment::Left);
    }
}

void ProgressBarNode::setLabelPersent(bool value)
{
    _labelPersent = value;
}

void ProgressBarNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_progressBar, states);
    target.draw(*_valueLabel, states);
}

void ProgressBarNode::setValue(float value)
{
    _progressBar.setRatio(value/_maxValue);

    if(_labelPersent)
        _valueLabel->setString(stringFormat("%.1f%%", _progressBar.getRatio()*100));
    else
        _valueLabel->setString(stringFormat("%.1f", value));

    auto pos = _valueLabel->getPosition();
    _valueLabel->setPosition(pos.x, -_progressBar.getSize().x * _progressBar.getRatio());
}
