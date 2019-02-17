#ifndef STATESTACK_H_
#define STATESTACK_H_

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>


namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
	public:
		enum Action
		{
			Push,
			Pop,
			Clear,
		};


	public:		
		explicit StateStack(State::Context context);

		template <typename T>
		void registerState(States::ID stateID);
		template <typename T, typename Param1>
		void registerState(States::ID stateID, Param1 arg1);

		void update(sf::Time dt);
		void draw();
		void handleEvent(const sf::Event& event);

		void pushState(States::ID stateID);
		void popState();
		void clearStates();

		bool isEmpty() const;

	private:
		State::Ptr	createState(States::ID stateID);
		void applyPendingChanges();


	private:
		struct PendingChange
		{
			explicit PendingChange(Action action, States::ID stateID = States::None);

			Action action;
			States::ID stateID;
		};

	private:
		std::vector<State::Ptr> _stack;
		std::vector<PendingChange> _pendingList;

		State::Context _context;
		std::map<States::ID, std::function<State::Ptr()>> _factories;
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	_factories[stateID] = [this] ()
	{
		return State::Ptr(new T(*this, _context));
	};
}

template <typename T, typename Param1>
void StateStack::registerState(States::ID stateID, Param1 arg1)
{
	_factories[stateID] = [this, arg1] ()
	{
		return State::Ptr(new T(*this, _context, arg1));
	};
}

#endif // STATESTACK_H_
