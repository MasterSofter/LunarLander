#ifndef WORLD_H_
#define WORLD_H_

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "Command.h"
#include "BloomEffect.h"
#include "SoundPlayer.h"
#include "StarfieldNode.h"
#include "LunarModuleNode.h"
#include "TextNode.h"
#include "State.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
	class RenderTarget;
}

class ProgressBarNode;

class World : private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<World> Ptr;
		constexpr const static double g = -1.62;

	public:
		World(State::Context context);
		void update(sf::Time dt);
		void draw();

        sf::RenderTarget& target() const;
		TextureHolder& getTextures();
        FontHolder& getFonts();

		//sf::FloatRect getViewBounds() const;
		CommandQueue& getCommandQueue();
		sf::Vector2u getSize() const;

        bool hasLanded() const;
        bool hasSafeLanded() const;

private:
		void loadTextures();
		void updateSounds();
		void buildScene();

private:
	sf::View _moonView;
    State::Context _context;
	StarfieldNode* _starfield;
    SpriteNode* _moon;
	SpriteNode* _earth;
	LunarModuleNode* _lunarModule;
	TextNode* _vertVelocity;
	TextNode* _horzVelocity;
    TextNode* _thrust;
	ProgressBarNode* _progressAlt;
	ProgressBarNode* _progressFuel;

	SceneNode _sceneGraph;
	CommandQueue _commandQueue;
	bool _explosionSound;

	BloomEffect _bloomEffect;
	SpriteNode* _finishSprite;

public:
	const double PIXEL_PER_METER = 50.f;
	const double TEXT_HEIGHT = 30.f;
};

#endif // WORLD_H_
