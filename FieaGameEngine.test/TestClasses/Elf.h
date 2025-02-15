#pragma once

#include "Definitions.h"
#include "Attributed.h"

namespace Fiea::GameEngine::test
{
	class Elf : public Attributed
	{
		RTTI_DECLARATIONS(Elf, Attributed);

	private:
		static const size_t MAX_SPELLS = 5;

	public:
		Elf();
		~Elf() = default;
		Elf(const Elf&) = default;
		Elf(Elf&&) noexcept = default;
		virtual Elf& operator=(const Elf&) = default;
		virtual Elf& operator=(Elf&&) = default;

		static std::vector<Signature> get_signatures();

		[[nodiscard]] Elf* Clone() const override;

	protected:
		Elf(size_t);

		//private:
	public: // public for testing purposes
		int mAge; // ha this is funny because mAge is short for myAge but it also spells Mage
		// additionally, has an internal datum called "internalDatum"
	};
}