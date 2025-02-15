#include "pch.h"
#include "Mage.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Mage);

Mage::Mage() : Attributed(Mage::TypeIdClass()), mName("Generic Mage"), mHealth(10), mSpells{"Firebolt", "Icebolt", "Lightningbolt", "Mana shield", "Healing"}
{
}

Mage::Mage(size_t typeId) : Attributed(typeId), mName("Generic Mage"), mHealth(10), mSpells{"Firebolt", "Icebolt", "Lightningbolt", "Mana shield", "Healing"}
{
}

std::vector<Signature> Mage::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mName", DatumType::String, 1, offsetof(Mage, mName) },
		{ "mHealth", DatumType::Integer, 1, offsetof(Mage, mHealth) },
		{ "mSpells", DatumType::String, MAX_SPELLS, offsetof(Mage, mSpells) },
	};
}

Mage* Mage::Clone() const
{
	return new Mage(*this);
}
