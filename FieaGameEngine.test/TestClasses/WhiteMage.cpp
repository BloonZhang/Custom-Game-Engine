#include "pch.h"
#include "WhiteMage.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(WhiteMage);

WhiteMage::WhiteMage() : Mage(WhiteMage::TypeIdClass()), mSpecialtySpell("Party Heal")
{
}


std::vector<Signature> WhiteMage::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mSpecialtySpell", DatumType::String, 1, offsetof(WhiteMage, mSpecialtySpell) },
	};
}

WhiteMage* WhiteMage::Clone() const
{
	return new WhiteMage(*this);
}
