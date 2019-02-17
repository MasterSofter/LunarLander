#include "SoundNode.h"
#include "SoundPlayer.h"


SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode(Category::SoundEffect),
	_sounds(player)
{
}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
	_sounds.play(sound, position);
}

