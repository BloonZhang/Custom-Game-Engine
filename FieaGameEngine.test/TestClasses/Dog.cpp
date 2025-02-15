#include "pch.h"
#include "Dog.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Dog);

void Dog::SubscribeToPerson(Person& person)
{
	using namespace std::placeholders;
	person.ThrowBoneEvent.Subscribe(this, std::bind(&Dog::FetchBone, this, _1, _2));
}

void Dog::SetMaximumMass(float mass)
{
	maximumMass = mass;
}

std::vector<std::string> Dog::GetBonesEaten() const
{
	return bonesEaten;
}

void Dog::FetchBone(RTTI* sender, const EventArgs* args)
{
	const ThrowBoneEventArgs* boneArgs = args->As<ThrowBoneEventArgs>();
	if (boneArgs->MassOfBone < maximumMass)
	{
		bonesEaten.push_back(boneArgs->ColorOfBone);
	}
}
