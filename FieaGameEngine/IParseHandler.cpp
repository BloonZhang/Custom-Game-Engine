#include "pch.h"
#include "IParseHandler.h"

using namespace Fiea::GameEngine;

void IParseHandler::initialize_handler()
{
	mIsActive = true;
}

void IParseHandler::cleanup_handler()
{
	mIsActive = false;
}

bool IParseHandler::is_active() const
{
	return mIsActive;
}
