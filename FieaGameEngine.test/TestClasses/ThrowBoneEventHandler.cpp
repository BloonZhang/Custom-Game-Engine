#include "pch.h"
#include "ThrowBoneEventHandler.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(ThrowBoneEventArgs);
RTTI_DEFINITIONS(ThrowBoneEventHandler);

ThrowBoneEventArgs::ThrowBoneEventArgs() : ColorOfBone("DefaultColor"), MassOfBone(0)
{

}

ThrowBoneEventArgs::ThrowBoneEventArgs(std::string color, float mass) : ColorOfBone(color), MassOfBone(mass)
{

}