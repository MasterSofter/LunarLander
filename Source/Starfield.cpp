#include "Starfield.h"
#include <random>
#include <functional>

std::mt19937 randomGenerator;
std::uniform_int_distribution<unsigned short int> randomDistributionAlpha(1u, 255u);
std::function <unsigned short int()> randomAlpha;

inline void randomSeed()
{
    std::random_device rd;
    randomGenerator.seed(rd());
    randomAlpha = std::bind(randomDistributionAlpha, randomGenerator);
}

inline float randomValue(const float low, const float high)
{
    return std::uniform_real_distribution<float>{low, high}(randomGenerator);
}

Starfield::Starfield(const sf::Vector2f size, const unsigned int numberOfStars, const sf::Color color)
        : _primitiveType{ sf::PrimitiveType::Points }
        , _vertices(numberOfStars)
        , _size(size)
        , _color(color)
{
    randomSeed();
    regenerate();
}

void Starfield::move(sf::Vector2f movement)
{
    for (auto& vertex : _vertices)
    {
        // move
        vertex.position += movement * (static_cast<float>(vertex.color.a) / 255.f);

        // wrap
        if (vertex.position.x < 0)
            vertex.position = { _size.x, randomValue(0.f, _size.y), };
        else if (vertex.position.x > _size.x)
            vertex.position = { 0.f, randomValue(0.f, _size.y), };
        if (vertex.position.y < 0)
            vertex.position = { randomValue(0.f, _size.x), _size.y, };
        else if (vertex.position.y > _size.y)
            vertex.position = { randomValue(0.f, _size.x) , 0.f };
    }
}

void Starfield::regenerate()
{
    for (auto& vertex : _vertices)
    {
        vertex.position = { randomValue(0.f, _size.x), randomValue(0.f, _size.y) };
        vertex.color = _color;
        vertex.color.a = static_cast<sf::Uint8>(randomAlpha());
    }
}

void Starfield::regenerate(const sf::Vector2f size)
{
    _size = size;
    regenerate();
}

void Starfield::regenerate(const sf::Vector2f size, const unsigned int numberOfStars)
{
    _vertices.resize(numberOfStars);
    regenerate(size);
}

void Starfield::regenerate(const unsigned int numberOfStars)
{
    regenerate(_size, numberOfStars);
}

void Starfield::setColor(const sf::Color color)
{
    _color = color;
    for (auto& vertex : _vertices)
    {
        const sf::Uint8 alphaDepth{ vertex.color.a };
        vertex.color = _color;
        vertex.color.a = alphaDepth;
    }
}

void Starfield::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.transform *= getTransform();
    const unsigned int size = static_cast<unsigned int>(_vertices.size());
    if (size > 0)
        target.draw(&_vertices.front(), size, _primitiveType, states);
}