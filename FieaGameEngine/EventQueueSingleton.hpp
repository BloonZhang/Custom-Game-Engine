#pragma once

#include "Definitions.h"
#include "EventInstance.h"

namespace Fiea::GameEngine
{
	class EventQueueSingleton
	{
	public:
		inline static EventQueueSingleton* Instance;

		/// @brief initialize_singleton
		/// Initializes the singleton by created a new instance
		/// Throws an exception if it has already been initialized
		static void initialize_singleton()
		{
			// if already initialized, then throw an exception
			if (Instance != nullptr) { throw std::invalid_argument("Tried to initialize EventQueueSingleton when it has already been intialized."); }
			// Otherwise, create a new instance and set it as the singleton instance
			Instance = new EventQueueSingleton();
		}

		/// @brief destroy_singleton
		/// Frees the memory of the singleton instance
		static void destroy_singleton()
		{
			// TODO: clear out Q of any uncalled events?
			delete Instance;
			Instance = nullptr;
		}
	
		/// @brief add_to_event_queue
		/// Adds an event to the event queue, to be called during the equivalent of LateUpdate()
		/// @param callback : the callback function for the event
		/// @param sender : the object that sent the event
		/// @param args : the EventArgs payload
		/// @param delay : Delay, if any, to dispatch the event, in milliseconds
		static void add_to_event_queue(DelegateFunction callback, RTTI* sender, std::shared_ptr<const EventArgs> args, const Millis& delay = 0)
		{
			// Create EventInstance struct
			// TODO: this is extremely cursed. Why am I making a GameTime instance only to call one method on it and immediately discard it
			EventInstance* eventInstance = new EventInstance(callback, sender, args, GameClock::Current().Get(), delay);
			// Put that struct in the Q
			Instance->mEventQueue.push_back(eventInstance);
		}

		static void dispatch_event_queue()
		{
			// Create a vector for the remaining EventInstances
			std::vector<const EventInstance*> remainingQueue;
			// For each EventInstance in the Q
			Millis currentTime = GameClock::Current().Get();
			for (auto& it : Instance->mEventQueue)
			{
				// Check if it's time to dispatch yet
				if (currentTime >= it->timeCalled + it->delay)
				{
					// Dispatch the event
					ProcessEvent(it);
					// Delete the event
					delete it;
					it = nullptr;
				}
				// add to remaining Q if not
				else
				{
					remainingQueue.push_back(it);
				}
			}
			// Transfor remainingQ into mEventQ
			Instance->mEventQueue = std::move(remainingQueue);
		}

	private:
		static void ProcessEvent(const EventInstance* eventInstance)
		{
			// Calls the function with the specified payload
			DelegateFunction callback = eventInstance->callback;
			callback(eventInstance->sender, eventInstance->args.get());
		}

	private:
		EventQueueSingleton() = default;
		~EventQueueSingleton() = default;
		std::vector<const EventInstance*> mEventQueue;
	};
}