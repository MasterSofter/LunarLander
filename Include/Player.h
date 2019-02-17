#ifndef PLAYER_H_
#define PLAYER_H_

#include "Command.h"
#include "KeyBinding.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <map>


class CommandQueue;

class Player : private sf::NonCopyable
{
	public:
		typedef PlayerAction::Type Action;

		enum MissionStatus
		{
			MissionRunning,
			MissionSuccess,
			MissionFailure
		};


	public:
		Player(const KeyBinding* binding);

		void handleEvent(const sf::Event& event, CommandQueue& commands);
		void handleRealtimeInput(CommandQueue& commands);
		void setMissionStatus(MissionStatus status);

private:
		void initializeActions();

	private:
		const KeyBinding* _keyBindingPtr;
		std::map<Action, Command> _actionBinding;
		std::map<Action, bool> _actionProxies;
		MissionStatus _currentMissionStatus;
};

#endif // PLAYER_H_
