#pragma once

#include "Definitions.h"
#include "GameObject.h"

namespace Fiea::GameEngine::test
{
	class TimeSum : public GameObject
	{
		RTTI_DECLARATIONS(TimeSum, GameObject);
	public:
		TimeSum();
	private:
		virtual void Update(const GameTime&) override;

	public:
		Vector3 velocity;
		long long totalTime;
	private:
	};
}