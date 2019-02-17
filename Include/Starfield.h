#ifndef LUNARLANDER_STARFIELD_H
#define LUNARLANDER_STARFIELD_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class Starfield : public sf::Drawable, public sf::Transformable
{
public:
    Starfield(sf::Vector2f size = { 0.f, 0.f }, unsigned int numberOfStars = 100u, sf::Color color = sf::Color(160, 160, 160));
    void regenerate();
    void regenerate(sf::Vector2f size);
    void regenerate(sf::Vector2f size, unsigned int numberOfStars);
    void regenerate(unsigned int numberOfStars);

    void setColor(sf::Color color);

    void move(sf::Vector2f movement);

private:
    sf::PrimitiveType _primitiveType;
    std::vector<sf::Vertex> _vertices;
    sf::Vector2f _size;
    sf::Color _color;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif //LUNARLANDER_STARFIELD_H
