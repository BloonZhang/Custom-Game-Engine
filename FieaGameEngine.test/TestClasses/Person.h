#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "ThrowBoneEventHandler.h"

namespace Fiea::GameEngine::test
{
	class Person : public RTTI
	{
		RTTI_DECLARATIONS(Person, RTTI);

	public:
		Person() = default;

		void ThrowBone(std::string color, float mass);
		void ThrowBoneWithDelay(std::string color, float mass, long long delay);

	public:
		ThrowBoneEventHandler ThrowBoneEvent;
	};
}