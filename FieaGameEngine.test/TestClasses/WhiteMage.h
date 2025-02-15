#pragma once

#include "Definitions.h"
#include "Mage.h"

namespace Fiea::GameEngine::test
{
	class WhiteMage final : public Mage
	{
		RTTI_DECLARATIONS(WhiteMage, Mage);

	private:
		static const size_t MAX_SPELLS = 5;

	public:
		WhiteMage();
		~WhiteMage() = default;
		WhiteMage(const WhiteMage&) = default;
		WhiteMage(WhiteMage&&) noexcept = default;
		virtual WhiteMage& operator=(const WhiteMage&) = default;
		virtual WhiteMage& operator=(WhiteMage&&) = default;

		static std::vector<Signature> get_signatures();

		[[nodiscard]] WhiteMage* Clone() const override;

		//private:
	public: // public for testing purposes
		std::string mSpecialtySpell;
	};
}