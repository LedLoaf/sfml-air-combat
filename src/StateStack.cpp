#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	// iteration from highest state to lowest
	for (auto it = mStack.rbegin(); it != mStack.rend(); ++it)
	{
		// stop loop if state does not let control flow to underlying states
		if (!(*it)->update(dt))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto& state : mStack)
	{
		state->draw();
	}
}

void StateStack::handleEvent(const sf::Event& event)
{
	// iteration from highest state to lowest
	for (auto it = mStack.rbegin(); it != mStack.rend(); ++it)
	{
		// stop loop if state does not let control flow to underlying states
		if (!(*it)->handleEvent(event))
		{
			break;
		}
	}
	applyPendingChanges();
}

void StateStack::pushState(StateID stateID)
{
	mPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

std::unique_ptr<State> StateStack::createState(StateID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());
	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (auto change : mPendingList)
	{
		switch (static_cast<unsigned int>(change.action))
		{
			case static_cast<unsigned int>(Action::Push):
				mStack.push_back(createState(change.stateID));
				break;
			case static_cast<unsigned int>(Action::Pop):
				mStack.pop_back();
				break;
			case static_cast<unsigned int>(Action::Clear):
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateID stateID)
: action(action)
, stateID(stateID)
{
}
