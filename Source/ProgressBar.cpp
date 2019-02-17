#include "ProgressBar.h"

ProgressBar::ProgressBar(const sf::Vector2f size)
        :amount{ 0.f },
        showBar{ true },
        showBackground{ false },
        size(size),
        color(sf::Color::White),
        bar(4),
        backgroundAndFrame(size),
        texture{ nullptr },
        backgroundTexture{ nullptr },
        textureRectangle(),
        backgroundTextureRectangle()
{
    backgroundAndFrame.setFillColor(sf::Color::Black);
    backgroundAndFrame.setOutlineColor(sf::Color::White);
    backgroundAndFrame.setOutlineThickness(1.f);
}

void ProgressBar::setSize(const sf::Vector2f s)
{
    size = s;
    priv_updateGraphics();
}

void ProgressBar::setPercentage(const float percentage)
{
    setRatio(percentage / 100.f);
}

void ProgressBar::setRatio(const float ratio)
{
    amount = ratio;
    if (amount < 0.f)
        amount = 0.f;
    else if (amount > 1.f)
        amount = 1.f;
    priv_updateGraphics();
}

void ProgressBar::setFrameThickness(const float frameThickness)
{
    backgroundAndFrame.setOutlineThickness(frameThickness);
}

void ProgressBar::setFrameColor(const sf::Color frameColor)
{
    backgroundAndFrame.setOutlineColor(frameColor);
}

void ProgressBar::setBackgroundColor(const sf::Color backgroundColor)
{
    backgroundAndFrame.setFillColor(backgroundColor);
}

void ProgressBar::setColor(const sf::Color c)
{
    color = c;
    priv_updateGraphics();
}

void ProgressBar::setShowBar(const bool show)
{
    showBar = show;
}

void ProgressBar::setShowBackgroundAndFrame(const bool showBackgroundAndFrame)
{
    showBackground = showBackgroundAndFrame;
}

void ProgressBar::setTexture(const sf::Texture& t, const bool resetRect)
{
    texture = &t;
    if (resetRect)
    {
        textureRectangle.width = texture->getSize().x;
        textureRectangle.height = texture->getSize().y;
    }
    priv_updateGraphics();
}

void ProgressBar::setTexture()
{
    texture = nullptr;
    priv_updateGraphics();
}

void ProgressBar::setTextureRect(const sf::IntRect& rect)
{
    textureRectangle = rect;
    priv_updateGraphics();
}

void ProgressBar::setBackgroundTexture(const sf::Texture& background, const bool resetRect)
{
    backgroundTexture = &background;
    if (resetRect)
    {
        backgroundTextureRectangle.width = backgroundTexture->getSize().x;
        backgroundTextureRectangle.height = backgroundTexture->getSize().y;
    }
    priv_updateGraphics();
}

void ProgressBar::setBackgroundTexture()
{
    backgroundTexture = nullptr;
    priv_updateGraphics();
}

void ProgressBar::setBackgroundTextureRect(const sf::IntRect& rect)
{
    backgroundTextureRectangle = rect;
    priv_updateGraphics();
}

const sf::Texture& ProgressBar::getTexture() const
{
    return *texture;
}

const sf::Texture& ProgressBar::getBackgroundTexture() const
{
    return *backgroundTexture;
}

sf::FloatRect ProgressBar::getLocalBounds() const
{
    if (showBackground && backgroundAndFrame.getOutlineThickness() > 0.f)
    {
        const float outlineThickness{ backgroundAndFrame.getOutlineThickness() };
        return{ { 0.f - outlineThickness, 0.f - outlineThickness }, { size.x + outlineThickness * 2, size.y + outlineThickness * 2 } };
    }
    else
        return{ { 0.f, 0.f }, size };
}

sf::FloatRect ProgressBar::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

sf::Vector2f ProgressBar::getAnchorProgressTop() const
{
    return getTransform().transformPoint({ size.x * amount, 0.f });
}

sf::Vector2f ProgressBar::getAnchorProgressCenter() const
{
    return getTransform().transformPoint({ size.x * amount, size.y / 2.f });
}

sf::Vector2f ProgressBar::getAnchorProgressBottom() const
{
    return getTransform().transformPoint({ size.x * amount, size.y });
}

// PRIVATE
void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if (showBackground)
        target.draw(backgroundAndFrame, states);

    if (showBar)
    {
        states.texture = texture;
        target.draw(&bar.front(), 4, sf::PrimitiveType::Quads, states);
    }
}

void ProgressBar::priv_updateGraphics()
{
    backgroundAndFrame.setSize(size);
    backgroundAndFrame.setTexture(backgroundTexture);
    backgroundAndFrame.setTextureRect(backgroundTextureRectangle);

    backgroundAndFrame.setSize(size);
    backgroundAndFrame.setTexture(backgroundTexture);
    backgroundAndFrame.setTextureRect(backgroundTextureRectangle);

    const float width{ size.x * amount };
    bar[0].position = { 0.f, 0.f };
    bar[1].position = { width, 0.f };
    bar[2].position = { width, size.y };
    bar[3].position = { 0.f, size.y };
    sf::FloatRect textureRect{ textureRectangle };
    textureRect.width = textureRect.width * amount;
    bar[0].texCoords = { textureRect.left, textureRect.top };
    bar[1].texCoords = { textureRect.left + textureRect.width, textureRect.top };
    bar[2].texCoords = { textureRect.left + textureRect.width, textureRect.top + textureRect.height };
    bar[3].texCoords = { textureRect.left, textureRect.top + textureRect.height };
    for (auto& vertex : bar)
        vertex.color = color;
}
