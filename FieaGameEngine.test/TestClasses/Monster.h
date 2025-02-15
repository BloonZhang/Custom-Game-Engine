#pragma once

#include "Definitions.h"
#include "Attributed.h"
#include "FactoryManagerSingleton.hpp"


namespace Fiea::GameEngine::test
{
	class Monster : public Attributed {
		RTTI_DECLARATIONS(Monster, Attributed);

	public:
		Monster();
		~Monster() = default;
		Monster(const Monster&) = default;
		Monster(Monster&&) = default;
		Monster& operator=(const Monster&) = default;
		Monster& operator=(Monster&&) = default;

		[[nodiscard]] virtual Monster* Clone() const override;

		static std::vector<Signature> get_signatures();

	public:
		int monsterHP;
		std::string monsterName;
	};

	MAKE_CONCRETE_FACTORY(Monster);
}