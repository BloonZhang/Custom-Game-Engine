#pragma once

#include "Definitions.h"
#include "Attributed.h"
#include "FactoryManagerSingleton.hpp"


namespace Fiea::GameEngine::test
{
	class MonsterArray : public Attributed {
		RTTI_DECLARATIONS(MonsterArray, Attributed);

	public:
		MonsterArray();
		~MonsterArray() = default;
		MonsterArray(const MonsterArray&) = default;
		MonsterArray(MonsterArray&&) = default;
		MonsterArray& operator=(const MonsterArray&) = default;
		MonsterArray& operator=(MonsterArray&&) = default;

		[[nodiscard]] virtual MonsterArray* Clone() const override;

		static std::vector<Signature> get_signatures();

	public:
		int intArray[3];
		float floatArray[3];
		glm::vec4 vecArray[3];
		glm::mat4 matArray[3];
		std::string strArray[3];
	};

	MAKE_CONCRETE_FACTORY(MonsterArray);
}