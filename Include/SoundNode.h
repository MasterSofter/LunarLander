#ifndef SOUNDNODE_H_
#define SOUNDNODE_H_

#include "SceneNode.h"
#include "ResourceIdentifiers.h"


class SoundPlayer;

class SoundNode : public SceneNode
{
	public:
		explicit SoundNode(SoundPlayer& player);
		void playSound(SoundEffect::ID sound, sf::Vector2f position);

	private:
		SoundPlayer& _sounds;
};

#endif // SOUNDNODE_H_
