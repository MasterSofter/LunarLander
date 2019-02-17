#include "SceneNode.h"
#include "Command.h"
#include "Utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


SceneNode::SceneNode(Category::Type category)
	: _children(),
	_parentPtr(nullptr),
	_defaultCategoryType(category)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->_parentPtr = this;
	_children.push_back(std::move(child));
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for(Ptr& child : _children)
		child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);

	// Draw bounding rectangle - disabled by default
	//drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(auto& child : _children)
		child->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->_parentPtr)
		transform = node->getTransform() * transform;

	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, dt);

	// Command children
	for(Ptr& child : _children)
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return _defaultCategoryType;
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for(Ptr& child : sceneGraph._children)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for(Ptr& child : _children)
		child->checkNodeCollision(node, collisionPairs);
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
	// By default, remove node if entity is destroyed
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	// By default, scene node needn't be removed
	return false;
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
