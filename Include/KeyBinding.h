#ifndef KEYBINDING_H_
#define KEYBINDING_H_

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>
#include <SFML/Window/Event.hpp>


namespace PlayerAction
{
	enum Type
	{
		ThrustOn,
		ThrustOff,
		ThrustHalf,
		ThrustPlus,
		ThrustMinus,
		TorqueRithtOn,
		TorqueLeftOn,
		TorqueOff,
		Count
	};
}

class KeyBinding
{
	public:
		typedef PlayerAction::Type Action;

	public:
		explicit KeyBinding();

		//void assignKey(Action action, sf::Keyboard::Key key);
		//sf::Keyboard::Key getAssignedKey(Action action) const;

		bool checkAction(const sf::Event& event, Action& out) const;
		std::vector<Action> getRealtimeActions() const;

	private:
		void initializeActions();

	private:
		std::map<sf::Keyboard::Key, Action>	_keyPressedMap;
		std::map<sf::Keyboard::Key, Action>	_keyRealesedMap;
};

bool isRealtimeAction(PlayerAction::Type action);

#endif // KEYBINDING_H_
