#include "KeyBinding.h"

#include <string>
#include <algorithm>
#include <SFML/Window/Event.hpp>


KeyBinding::KeyBinding()
	: _keyPressedMap(), _keyRealesedMap()
{
	_keyPressedMap[sf::Keyboard::Right]   = PlayerAction::TorqueRithtOn;
	_keyPressedMap[sf::Keyboard::Left]    = PlayerAction::TorqueLeftOn;

    _keyRealesedMap[sf::Keyboard::Up]     = PlayerAction::ThrustPlus;
    _keyRealesedMap[sf::Keyboard::Down]   = PlayerAction::ThrustMinus;
    _keyRealesedMap[sf::Keyboard::Space]  = PlayerAction::ThrustOn;
    _keyRealesedMap[sf::Keyboard::Delete] = PlayerAction::ThrustOff;
    _keyRealesedMap[sf::Keyboard::F5] 	  = PlayerAction::ThrustHalf;
    _keyRealesedMap[sf::Keyboard::Right]  = PlayerAction::TorqueOff;
	_keyRealesedMap[sf::Keyboard::Left]   = PlayerAction::TorqueOff;
}

bool KeyBinding::checkAction(const sf::Event& event, Action& out) const
{
	auto keyMap = event.type == sf::Event::KeyPressed
			? _keyPressedMap
			: _keyRealesedMap;

	auto found = keyMap.find(event.key.code);

	if (found == keyMap.end())
		return false;

	out = found->second;
	return true;
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const
{
	// Return all realtime actions that are currently active.
	std::vector<Action> actions;

	for(auto& pair : _keyPressedMap)
	{
		// If key is pressed and an action is a realtime action, store it
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			actions.push_back(pair.second);
	}

	return actions;
}

bool isRealtimeAction(PlayerAction::Type action)
{
	switch (action)
	{
		case PlayerAction::ThrustOn:
		case PlayerAction::ThrustOff:
		case PlayerAction::ThrustHalf:
		case PlayerAction::ThrustPlus:
		case PlayerAction::ThrustMinus:
		case PlayerAction::TorqueOff:
		case PlayerAction::TorqueLeftOn:
		case PlayerAction::TorqueRithtOn:
			return true;

		default:
			return false;
	}
}