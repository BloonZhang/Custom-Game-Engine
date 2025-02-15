#include "pch.h"
#include "Person.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Person);

void Person::ThrowBone(std::string color, float mass)
{
	ThrowBoneEventArgs* args = new ThrowBoneEventArgs(color, mass);
	ThrowBoneEvent.Invoke(this, args);
}

void Person::ThrowBoneWithDelay(std::string color, float mass, long long delay)
{
	ThrowBoneEventArgs* args = new ThrowBoneEventArgs(color, mass);
	ThrowBoneEvent.InvokeDelayed(this, args, delay);
}
