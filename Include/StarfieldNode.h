#ifndef LUNARLANDER_STARFIELDNODE_H
#define LUNARLANDER_STARFIELDNODE_H



#include "Starfield.h"
#include "SceneNode.h"
#include "Utility.h"

class StarfieldNode : public SceneNode
{
    Starfield _starfield;

public:
    StarfieldNode(sf::Vector2f size);
    void move(Vector2d velocity, sf::Time dt);

private:

    //virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif //LUNARLANDER_STARFIELDNODE_H
