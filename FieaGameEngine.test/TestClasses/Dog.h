#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "Person.h"

namespace Fiea::GameEngine::test
{
	class Dog : public RTTI
	{
		RTTI_DECLARATIONS(Dog, RTTI);
	public:
		Dog() = default;
		// Debug code for subscribing FetchBone to a person's ThrowBoneEvent
		void SubscribeToPerson(Person& person);
		void SetMaximumMass(float mass);
		std::vector<std::string> GetBonesEaten() const;

	private:
		void FetchBone(RTTI* sender, const EventArgs* args);

	private:
		float maximumMass = 100;
		std::vector<std::string> bonesEaten;
	};
}