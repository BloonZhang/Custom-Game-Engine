#include "pch.h"
#include "ChildOfGameObject.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(ChildOfGameObject);

ChildOfGameObject::ChildOfGameObject() : GameObject(ChildOfGameObject::TypeIdClass()), mInt(0)
{
}

std::vector<Signature> Fiea::GameEngine::test::ChildOfGameObject::get_signatures()
{
	return std::vector<Signature>{
		{ "mInt", DatumType::Integer, 1, offsetof(ChildOfGameObject, mInt) }
	};
}

ChildOfGameObject* Fiea::GameEngine::test::ChildOfGameObject::Clone() const
{
	return new ChildOfGameObject(*this);
}
