#pragma once

#include "Definitions.h"
#include "GameObject.h"

namespace Fiea::GameEngine::test
{
	class ChildOfGameObject : public GameObject
	{
		RTTI_DECLARATIONS(ChildOfGameObject, GameObject);

	public:
		ChildOfGameObject();
		inline int get_int() const { return mInt; }
		// Inherited
		static std::vector<Signature> get_signatures();
		[[nodiscard]] virtual ChildOfGameObject* Clone() const override;
	private:
		int mInt;
	};
}