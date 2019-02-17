#include "World.h"
#include "TextNode.h"
#include "SoundNode.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <StarfieldNode.h>
#include "ProgressBarNode.h"
#include "TextNode.h"
#include "ProgressBarNode.h"
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>


World::World(State::Context context) :
	_context(context),
	_sceneGraph(),
	_finishSprite(nullptr),
	_explosionSound(true),
	_moonView(context.window->getDefaultView())
{
	buildScene();
}

sf::RenderTarget& World::target() const
{
	return *_context.window;
}

bool World::hasLanded() const
{
	return _lunarModule->hasLanded();
}

bool World::hasSafeLanded() const
{
	return _lunarModule->hasSafeLanded();
}

void World::update(sf::Time dt)
{
	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!_commandQueue.isEmpty())
		_sceneGraph.onCommand(_commandQueue.pop(), dt);

	// Regular update step, adapt position (correct if outside view)
	_sceneGraph.update(dt, _commandQueue);

	auto vel = _lunarModule->getVelocity();

	if(!_lunarModule->hasLanded())
	{
		_moon->setPosition(0, _lunarModule->getPosition().y - 450 + _lunarModule->getPos().y * PIXEL_PER_METER);
		_starfield->move(vel, dt);
	}

	if(_lunarModule->hasLanded())
    {
        if(!_lunarModule->hasSafeLanded() && _explosionSound) {
			_context.sounds->play(SoundEffect::ID::Explosion);
			_explosionSound = false;
        }
    }

	_moonView.move(vel.x * dt.asSeconds() * PIXEL_PER_METER, 0);

	_vertVelocity->setString(stringFormat("%.1f", vel.y));
	_horzVelocity->setString(stringFormat("%.1f", vel.x));

	if(_lunarModule->getThrust() > 0.f)
		_thrust->setString(toString(_lunarModule->getThrust() / _lunarModule->maxThrust() * 100.f) + "%");
	else
		_thrust->setString("OFF");

	_progressAlt->setValue(_lunarModule->getPos().y);
	_progressFuel->setValue(_lunarModule->getFuel());

	updateSounds();
}

void World::draw()
{
//	target().setView(target().getDefaultView());

	for(auto& node : _sceneGraph.getChildren())
	{
		if(node->getCategory() == Category::Moon)
		{
			target().setView(_moonView);
			target().draw(*node);
			target().setView(target().getDefaultView());
		}
		else
			target().draw(*node);
	}
}

CommandQueue& World::getCommandQueue()
{
	return _commandQueue;
}

TextureHolder& World::getTextures()
{
	return *_context.textures;
}

FontHolder& World::getFonts()
{
	return *_context.fonts;
}

sf::Vector2u World::getSize() const
{
	return target().getSize();
}

void World::updateSounds()
{
	sf::Vector2f listenerPosition;

	// Set listener's position
	_context.sounds->setListenerPosition(listenerPosition);

	// Remove unused sounds
	_context.sounds->removeStoppedSounds();
}

void World::buildScene()
{
    SceneNode::Ptr layerBackground(new StarfieldNode(sf::Vector2f(getSize().x, getSize().y)));
	_starfield = (StarfieldNode*)layerBackground.get();
	_sceneGraph.attachChild(std::move(layerBackground));

    std::unique_ptr<SpriteNode> earthSprite(new SpriteNode(getTextures().get(Textures::Earth)));
    centerOrigin(earthSprite->getSprite());
	earthSprite->setScale(0.5, 0.5);
    earthSprite->setPosition(target().getSize().x - target().getSize().x/5.f, target().getSize().y/4.f);
    _earth = earthSprite.get();
    _sceneGraph.attachChild(std::move(earthSprite));

	sf::Texture& moonTexture = getTextures().get(Textures::Moon);
	moonTexture.setRepeated(true);

    sf::IntRect moonRect(0, 0, target().getSize().x*4000, moonTexture.getSize().y);
    std::unique_ptr<SpriteNode> moonSprite(new SpriteNode(moonTexture, moonRect, Category::Moon));
	moonSprite->setOrigin(0.f, moonTexture.getSize().y/2.f);
    _moon = moonSprite.get();
	_moonView.setCenter(moonRect.width / 2.f, 0);
    _sceneGraph.attachChild(std::move(moonSprite));

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// лунный модуль
	LunarModuleNode::Ptr lunarModule(new LunarModuleNode(this));
	lunarModule->LoadConfig("appolo.config");

	_lunarModule = lunarModule.get();
	_sceneGraph.attachChild(std::move(lunarModule));

	const sf::Vector2f padding(50.f, TEXT_HEIGHT);
	const sf::Vector2f progressSize(target().getSize().y/2.f, 15.f);
	const sf::Color color(5, 98, 1, 255);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Высота
	ProgressBarNode::Ptr progressAlt(new ProgressBarNode(*this, _lunarModule->getInitPos().y, progressSize, -90));
	progressAlt->setPosition(sf::Vector2f(padding.x + progressSize.y/2.f, getSize().y - (getSize().y - progressSize.x)/2.f - 50));
	progressAlt->setLabelSide(TextNode::Alignment::Right);

	std::unique_ptr<TextNode> labelAlt(new TextNode("ALT", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	labelAlt->setPosition(0, padding.y);
	labelAlt->setColor(color);
	progressAlt->attachChild(std::move(labelAlt));
	_progressAlt = progressAlt.get();
	_sceneGraph.attachChild(std::move(progressAlt));

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Топливо
	ProgressBarNode::Ptr progressFuel(new ProgressBarNode(*this, _lunarModule->getInitFuel(), progressSize, -90, true));
	progressFuel->setPosition(sf::Vector2f(getSize().x - padding.x - progressSize.y/2.f, getSize().y - (getSize().y - progressSize.x)/2.f - 50));
	progressFuel->setLabelSide(TextNode::Alignment::Left);
	progressFuel->setLabelPersent(true);

	std::unique_ptr<TextNode> labelFuel(new TextNode("FUEL", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	labelFuel->setPosition(0, padding.y);
	labelFuel->setColor(color);
	progressFuel->attachChild(std::move(labelFuel));
	_progressFuel = progressFuel.get();
	_sceneGraph.attachChild(std::move(progressFuel));

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Вертикальная скорость
	auto offsetX = getSize().x/4.f - TEXT_HEIGHT;
	std::unique_ptr<TextNode> label(new TextNode("VERT:", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	label->setPosition(offsetX, padding.y);
	label->setColor(color);

	std::unique_ptr<TextNode> text(new TextNode("", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	text->setPosition(label->getBoundingRect().width, 0.f);
	text->setColor(color);
	text->setAlignment(TextNode::Alignment::Left);
	_vertVelocity = text.get();
	label->attachChild(std::move(text));
	_sceneGraph.attachChild(std::move(label));

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Горизонтальная скорость
	label = std::unique_ptr<TextNode>(new TextNode("HORZ:", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	label->setPosition(2*offsetX, padding.y);
	label->setColor(color);

	text = std::unique_ptr<TextNode>(new TextNode("", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	text->setPosition(label->getBoundingRect().width, 0.f);
	text->setColor(color);
	text->setAlignment(TextNode::Alignment::Left);
	_horzVelocity = text.get();
	label->attachChild(std::move(text));
	_sceneGraph.attachChild(std::move(label));

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Тяга двигателя
	label = std::unique_ptr<TextNode>(new TextNode("THRUST:", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	label->setPosition(3*offsetX, padding.y);
	label->setColor(color);

	text = std::unique_ptr<TextNode>(new TextNode("", getFonts().get(Fonts::Main), TEXT_HEIGHT));
	text->setPosition(label->getBoundingRect().width, 0.f);
	text->setColor(color);
	text->setAlignment(TextNode::Alignment::Left);
	_thrust = text.get();
	label->attachChild(std::move(text));
	_sceneGraph.attachChild(std::move(label));


	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(*_context.sounds));
	_sceneGraph.attachChild(std::move(soundNode));
}
