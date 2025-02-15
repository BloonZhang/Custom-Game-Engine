#pragma once

#include "Definitions.h"

namespace Fiea::GameEngine
{
	// Forward declaration for friendship
	class EventQueueSingleton;

	// private struct only for EventQueueSingleton
	class EventInstance
	{
		friend EventQueueSingleton;
	private:
		EventInstance(DelegateFunction callback, RTTI* sender, std::shared_ptr<const EventArgs> args, const Millis& timeCalled, const Millis& delay);
		~EventInstance();
		DelegateFunction callback;
		RTTI* sender;
		std::shared_ptr<const EventArgs> args;
		const Millis timeCalled;
		const Millis delay;
	};
}