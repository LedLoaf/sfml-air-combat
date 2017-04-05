#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <map>

//class State;

class StateStack : private sf::NonCopyable
{
	public:
		enum class Action
		{
			Push,
			Pop,
			Clear
		};

	public:
		explicit					StateStack(State::Context context);

		template <typename T>
		void						registerState(StateID stateID);

		void						update(sf::Time dt);
		void						draw();
		void						handleEvent(const sf::Event& event);

		void						pushState(StateID stateID);
		void						popState();
		void						clearStates();

		bool						isEmpty() const;

	private:
		std::unique_ptr<State>		createState(StateID stateID);
		void						applyPendingChanges();

	private:
		struct PendingChange
		{
			explicit	PendingChange(Action action, StateID stateID = StateID::None);
			Action		action;
			StateID		stateID;
		};

	private:
		std::vector<std::unique_ptr<State>>		mStack;
		std::vector<PendingChange>				mPendingList;
		State::Context							mContext;
		std::map<StateID, std::function<std::unique_ptr<State>()>>	mFactories;
};

template <typename T>
void StateStack::registerState(StateID stateID)
{
	mFactories[stateID] = [this] ()
	{
		return std::make_unique<T>(*this, mContext);
	};
}

#endif // STATESTACK_HPP

