#include "pch.h"
#include "IntHandler.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

void IntHandler::initialize_handler()
{
}

void IntHandler::cleanup_handler()
{
}

bool IntHandler::start_handler(Wrapper* wrapper, JsonPair jsonPair)
{
	// check wrapper
	IntWrapper* intWrapper = wrapper->As<IntWrapper>();
	if (intWrapper == nullptr) { return false; }

	// Get the value
	Json::Value jsonValue = jsonPair.second;

	// Do checking if jsonValue is valid
	if (!CheckIfValid(jsonValue)) { return false; }

	// At this point, we know that jsonValue is associated with ints

	// If array
	if (jsonValue.isArray())
	{
		for (Json::Value::const_iterator::value_type it : jsonValue)
		{
			intWrapper->add_int(it.asInt());
		}
	}
	// If not array
	else
	{
		intWrapper->add_int(jsonValue.asInt());
	}
	// We're done here. Return true
	return true;
}

bool IntHandler::end_handler(Wrapper* wrapper, JsonPair jsonPair)
{
	return false;
}

bool IntHandler::CheckIfValid(Json::Value jsonValue) const
{
	// If array
	if (jsonValue.isArray())
	{
		// Check for empty array
		if (jsonValue.size() == 0) { return false; }
		// Check if integer array
		if (!jsonValue[0].isInt()) { return false; }
	}
	// If not array
	else
	{
		// Check if integer
		if (!jsonValue.isInt()) { return false; }
	}
	// Good to go
	return true;
}
