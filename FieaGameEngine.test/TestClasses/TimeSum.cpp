#include "pch.h"
#include "TimeSum.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(TimeSum);

TimeSum::TimeSum() : totalTime(0), velocity(0, 0, 0)
{
}

void TimeSum::Update(const GameTime& deltaTime)
{
	totalTime += deltaTime.Frame();
	setLocalPosition(localPosition() + velocity);
	GameObject::Update(deltaTime);
}
