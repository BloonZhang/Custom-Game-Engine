#include "pch.h"
#include "EventInstance.h"
#include "EventHandler.h"

using namespace Fiea::GameEngine;

EventInstance::EventInstance(DelegateFunction callback, RTTI* sender, std::shared_ptr<const EventArgs> args, const Millis& timeCalled, const Millis& delay) :
	callback(callback),
	sender(sender), 
	args(args), /* NOTE: gains shared ownership of args */
	timeCalled(timeCalled),
	delay(delay)
{}

EventInstance::~EventInstance()
{
	// args is now a shared pointer. No need to delete
	//delete args;
}