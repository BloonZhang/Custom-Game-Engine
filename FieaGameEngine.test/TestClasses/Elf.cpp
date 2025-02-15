#include "pch.h"
#include "Elf.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Elf);

std::vector<Signature> Elf::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mAge", DatumType::Integer, 1, offsetof(Elf, mAge) },
		{ "internalDatum", DatumType::Table, 0, 0 }
	};
}

Elf* Elf::Clone() const
{
	return new Elf(*this);
}

Elf::Elf() : Attributed(Elf::TypeIdClass()), mAge(1000)
{
}

Elf::Elf(size_t typeId) : Attributed(typeId), mAge(1000)
{
}