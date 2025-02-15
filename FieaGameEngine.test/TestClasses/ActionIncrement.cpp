#include "pch.h"
#include "ActionIncrement.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(ActionIncrement);

ActionIncrement::ActionIncrement() : Action(ActionIncrement::TypeIdClass()), mNameOfTargetDatum("NONE"), mIncrementAmount(0)
{
}

#pragma region Inherited
void ActionIncrement::Update(const GameTime& deltaTime)
{
	// Find the datum with the relevant key
	Datum* target = get_datum_in_parent_by_key(mNameOfTargetDatum, nullptr);
	// Error checking
	if (target == nullptr) 
	{ 
		throw std::runtime_error("Datum with key " + mNameOfTargetDatum + " not found in ActionIncrement::Update()"); 
	}
	if (!target->is_compatible_with(DatumType::Integer))
	{
		throw std::runtime_error("Datum with key " + mNameOfTargetDatum + " is not compatible with Integers in ActionIncrement::Update()");
	}
	// Increment the value in the datum
	target->get_int_at(0) += mIncrementAmount;
}

std::vector<Signature> ActionIncrement::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mNameOfTargetDatum", DatumType::String, 1, offsetof(ActionIncrement, mNameOfTargetDatum) },
		{ "mIncrementAmount", DatumType::Integer, 1, offsetof(ActionIncrement, mIncrementAmount) }
	};
}

ActionIncrement* ActionIncrement::Clone() const
{
	return new ActionIncrement(*this);
}
#pragma endregion

void ActionIncrement::set_name_of_target_datum(const std::string& target)
{
	mNameOfTargetDatum = target;
}

void ActionIncrement::set_increment_amount(int amount)
{
	mIncrementAmount = amount;
}