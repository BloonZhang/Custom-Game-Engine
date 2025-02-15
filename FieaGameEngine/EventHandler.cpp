#include "pch.h"
#include "EventHandler.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(EventHandler);
RTTI_DEFINITIONS(EventArgs);

void EventHandler::Invoke(RTTI* sender, const EventArgs* args, Millis delay)
{
	// Turn args into a shared pointer because we're about to share ownership
	std::shared_ptr<const EventArgs> sharedPtr(args);
	// For each subscriber, add it to the event Q
	for (auto i : mListOfCallbackFunctions)
	{
		for (auto j : i.second)
		{
			EventQueueSingleton::add_to_event_queue(j, sender, sharedPtr, delay);
		}
	}
}

void EventHandler::InvokeDelayed(RTTI* sender, const EventArgs* args, Millis delay)
{
	Invoke(sender, args, delay);
}

//void EventHandler::operator+=(DelegateFunction callback)
/*
void EventHandler::Subscribe(DelegateFunction callback)
{
	// Something just subscribed! Add to list of subscribers
	mListOfCallbackFunctions.push_back(callback);
}
*/
void EventHandler::Subscribe(RTTI * subscriber, DelegateFunction callback)
{
	mListOfCallbackFunctions[subscriber].push_back(callback);
}

void EventHandler::UnsubscribeAll(RTTI* subscriber)
{
	mListOfCallbackFunctions.erase(subscriber);
}

/*
void EventHandler::Subscribe(RTTI* subscriber, DelegateFunction* callback)
{
	using namespace std::placeholders;

	auto f = std::bind(callback, subscriber, _1, _2);
	f(nullptr, nullptr);

	//auto asdf = [&subscriber, &callback](auto first, auto second) { callback(first, second); };
	//EventQueueSingleton::TestMethod(asdf);
}
*/