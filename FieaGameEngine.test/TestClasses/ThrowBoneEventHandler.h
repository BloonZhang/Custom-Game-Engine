#pragma once

#include "Definitions.h"
#include "EventHandler.h"

namespace Fiea::GameEngine::test
{
	class ThrowBoneEventArgs : public EventArgs
	{
		RTTI_DECLARATIONS(ThrowBoneEventArgs, EventArgs);
	public:
		ThrowBoneEventArgs();
		ThrowBoneEventArgs(std::string, float);

	public:
		std::string ColorOfBone;
		float MassOfBone;
	};


	class ThrowBoneEventHandler : public EventHandler
	{
		RTTI_DECLARATIONS(ThrowBoneEventHandler, EventHandler);
	public:

	private:
	};


}