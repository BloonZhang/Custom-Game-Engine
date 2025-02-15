#include "pch.h"
#include "Monster.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Monster);

Monster::Monster() : Attributed(Monster::TypeIdClass()), monsterHP(5), monsterName("Bob")
{

}

Monster* Monster::Clone() const
{
	return new Monster(*this);
}

std::vector<Signature> Monster::get_signatures()
{
	return std::vector<Signature>
	{
		{ "monsterHP", DatumType::Integer, 1, offsetof(Monster, monsterHP) },
		{ "monsterName", DatumType::String, 1, offsetof(Monster, monsterName) },
	};
}