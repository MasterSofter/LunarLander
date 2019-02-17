#include "Player.h"
#include "CommandQueue.h"
#include "LunarModuleNode.h"

#include <SFML/Network/Packet.hpp>

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

struct ThrustOn
{
	ThrustOn(double t) {thrust = t;}
	void operator() (LunarModuleNode& lunarModule, sf::Time) const {
		if(!lunarModule.hasLanded())
			lunarModule.setThrust(lunarModule.maxThrust() * thrust);
	}
	double thrust;
};

struct ThrustPlus
{
	ThrustPlus(double val) { delta = val; }
	void operator() (LunarModuleNode& lunarModule, sf::Time) const {
		if(!lunarModule.hasLanded())
			lunarModule.setThrust(lunarModule.getThrust() + lunarModule.maxThrust() * delta);
	}
	double delta;
};

struct TorqueRightOn
{
	void operator() (LunarModuleNode& lunarModule, sf::Time) const 	{
		if(!lunarModule.hasLanded())
			lunarModule.setTorque(-1);
	}
};

struct TorqueLeftOn
{
	void operator() (LunarModuleNode& lunarModule, sf::Time) const 	{
		if(!lunarModule.hasLanded())
			lunarModule.setTorque(1);
	}
};

struct TorqueOff
{
	void operator() (LunarModuleNode& lunarModule, sf::Time) const 	{
		lunarModule.setTorque(0);
	}
};


Player::Player(const KeyBinding* binding)
	: _keyBindingPtr(binding),
	  _currentMissionStatus(MissionRunning)
{
	initializeActions();
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	// Event
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
	{
		Action action;
		if (_keyBindingPtr && _keyBindingPtr->checkAction(event, action))
			commands.push(_actionBinding[action]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Lookup all actions and push corresponding commands to queue
	std::vector<Action> activeActions = _keyBindingPtr->getRealtimeActions();
	for(auto& action : activeActions)
		commands.push(_actionBinding[action]);
}

void Player::setMissionStatus(MissionStatus status)
{
	_currentMissionStatus = status;
}

void Player::initializeActions()
{
	_actionBinding[PlayerAction::ThrustOn].action      = derivedAction<LunarModuleNode>(ThrustOn(1.f));
	_actionBinding[PlayerAction::ThrustOff].action     = derivedAction<LunarModuleNode>(ThrustOn(0.f));
	_actionBinding[PlayerAction::ThrustHalf].action    = derivedAction<LunarModuleNode>(ThrustOn(0.5f));
	_actionBinding[PlayerAction::ThrustPlus].action    = derivedAction<LunarModuleNode>(ThrustPlus(0.1f));
	_actionBinding[PlayerAction::ThrustMinus].action   = derivedAction<LunarModuleNode>(ThrustPlus(-0.1f));
	_actionBinding[PlayerAction::TorqueRithtOn].action = derivedAction<LunarModuleNode>(TorqueRightOn());
	_actionBinding[PlayerAction::TorqueLeftOn].action  = derivedAction<LunarModuleNode>(TorqueLeftOn());
	_actionBinding[PlayerAction::TorqueOff].action     = derivedAction<LunarModuleNode>(TorqueOff());

	for(auto it = _actionBinding.begin(); it != _actionBinding.end(); ++it)
		it->second.category = Category::Type::SceneLunarModule;
}
