#pragma once

#include "Definitions.h"
#include "Attributed.h"

namespace Fiea::GameEngine::test
{
	class Mage : public Attributed
	{
		RTTI_DECLARATIONS(Mage, Attributed);

	private:
		static const size_t MAX_SPELLS = 5;

	public:
		Mage();
		~Mage() = default;
		Mage(const Mage&) = default;
		Mage(Mage&&) noexcept = default;
		virtual Mage& operator=(const Mage&) = default;
		virtual Mage& operator=(Mage&&) = default;

		static std::vector<Signature> get_signatures();

		[[nodiscard]] Mage* Clone() const override;

	protected:
		Mage(size_t);

	//private:
	public: // public for testing purposes
		std::string mName;
		int mHealth;
		std::string mSpells[MAX_SPELLS];
	};
}