#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "EventQueueSingleton.hpp"

namespace Fiea::GameEngine
{
	class EventArgs : public RTTI
	{
		RTTI_DECLARATIONS(EventArgs, RTTI);
	public:
		EventArgs() = default;
		virtual ~EventArgs() = default;
	};

	class EventHandler : public RTTI
	{
		RTTI_DECLARATIONS(EventHandler, RTTI);
	public:
		EventHandler() = default;
		virtual ~EventHandler() = default;
		EventHandler(const EventHandler&) = delete;
		EventHandler(EventHandler&&) = delete;
		EventHandler& operator=(const EventHandler&) = delete;
		EventHandler& operator=(EventHandler&&) = delete;

	public:
		void Invoke(RTTI* sender, const EventArgs* args, Millis delay = 0);
		void InvokeDelayed(RTTI* sender, const EventArgs* args, Millis delay);

		//void operator+=(DelegateFunction callback);
		//void Subscribe(DelegateFunction callback);
		void Subscribe(RTTI* subscriber, DelegateFunction callback);
		void UnsubscribeAll(RTTI* subscriber);
	private:
		//std::vector<DelegateFunction> mListOfCallbackFunctions;
		std::unordered_map<RTTI*, std::vector<DelegateFunction>> mListOfCallbackFunctions;
	};


}