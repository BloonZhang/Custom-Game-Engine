#include "pch.h"
#include "MonsterArray.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(MonsterArray);

MonsterArray::MonsterArray() : Attributed(MonsterArray::TypeIdClass())
{

}

MonsterArray* MonsterArray::Clone() const
{
	return new MonsterArray(*this);
}

std::vector<Signature> MonsterArray::get_signatures()
{
	return std::vector<Signature>
	{
		{ "intArray", DatumType::Integer, 3, offsetof(MonsterArray, intArray) },
		{ "floatArray", DatumType::Float, 3, offsetof(MonsterArray, floatArray) },
		{ "vecArray", DatumType::Vector4, 3, offsetof(MonsterArray, vecArray) },
		{ "matArray", DatumType::Matrix4, 3, offsetof(MonsterArray, matArray) },
		{ "strArray", DatumType::String, 3, offsetof(MonsterArray, strArray) }
	};
}