#ifndef LUNARLANDER_PROGRESSBAR_H
#define LUNARLANDER_PROGRESSBAR_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


//https://github.com/Hapaxia/SelbaWard

class ProgressBar : public sf::Drawable, public sf::Transformable
{
public:
    // creation and size
    ProgressBar(sf::Vector2f size = { 64.f, 8.f });
    void setSize(sf::Vector2f size);
    sf::Vector2f getSize() const;

    // progress
    void setRatio(float ratio);
    float getRatio() const;
    void setPercentage(float percentage);
    float getPercentage() const;
    template <class T>
    void setFromValueInRange(const T& value, const T& minimum, const T& maximum);
    template <class T>
    void setFromValueInRange(const T& value, const T& range);

    // visual representation
    void setFrameThickness(float frameThickness);
    void setFrameColor(sf::Color frameColor);
    void setBackgroundColor(sf::Color backgroundColor);
    void setColor(sf::Color color);
    void setShowBar(bool showBar);
    void setShowBackgroundAndFrame(bool showBackgroundAndFrame);
    float getFrameThickness() const;
    sf::Color getFrameColor() const;
    sf::Color getBackgroundColor() const;
    sf::Color getColor() const;
    bool getShowBar() const;
    bool getShowBackgroundAndFrame() const;

    // texturing
    void setTexture(const sf::Texture& texture, bool resetRect = false);
    void setTexture(); // clear/nullify texture
    void setTextureRect(const sf::IntRect& textureRectangle);
    const sf::Texture& getTexture() const;
    void setBackgroundTexture(const sf::Texture& backgroundTexture, bool resetRect = false);
    void setBackgroundTexture(); // clear/nullify background texture
    void setBackgroundTextureRect(const sf::IntRect& backgroundTextureRectangle);
    const sf::Texture& getBackgroundTexture() const;

    // bounds
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    // anchor points
    sf::Vector2f getAnchorProgressTop() const;
    sf::Vector2f getAnchorProgressCenter() const;
    sf::Vector2f getAnchorProgressBottom() const;

private:
    float amount;
    bool showBar;
    bool showBackground;
    sf::Vector2f size;
    sf::Color color;
    std::vector<sf::Vertex> bar;
    sf::RectangleShape backgroundAndFrame;
    const sf::Texture* texture;
    const sf::Texture* backgroundTexture;
    sf::IntRect textureRectangle;
    sf::IntRect backgroundTextureRectangle;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void priv_updateGraphics();
};

inline sf::Vector2f ProgressBar::getSize() const
{
    return size;
}

inline float ProgressBar::getRatio() const
{
    return amount;
}

inline float ProgressBar::getPercentage() const
{
    return amount * 100.f;
}

template <class T>
void ProgressBar::setFromValueInRange(const T& value, const T& minimum, const T& maximum)
{
    setRatio(static_cast<float>(value - minimum) / static_cast<float>(maximum - minimum));
}

template <class T>
void ProgressBar::setFromValueInRange(const T& value, const T& range)
{
    setRatio(static_cast<float>(value) / static_cast<float>(range));
}

inline float ProgressBar::getFrameThickness() const
{
    return backgroundAndFrame.getOutlineThickness();
}

inline sf::Color ProgressBar::getFrameColor() const
{
    return backgroundAndFrame.getOutlineColor();
}

inline sf::Color ProgressBar::getBackgroundColor() const
{
    return backgroundAndFrame.getFillColor();
}

inline sf::Color ProgressBar::getColor() const
{
    return color;
}

inline bool ProgressBar::getShowBar() const
{
    return showBar;
}

inline bool ProgressBar::getShowBackgroundAndFrame() const
{
    return showBackground;
}

#endif //LUNARLANDER_PROGRESSBAR_H
