#include "pch.h"
#include "ActionListWhile.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(ActionListWhile);

/// @brief Default constructor
/// Calls Attributed typeID constructor
Fiea::GameEngine::ActionListWhile::ActionListWhile() : ActionList(ActionListWhile::TypeIdClass()), mCondition(0)
{
}

/// @brief adopt_action_as_preabmel
/// Adds an action to the list of actions to run as the preamble
/// Will take memory ownership of the pointer
/// @param action - Pointer to the action to adopt
void ActionListWhile::adopt_action_as_preamble(Action* action)
{
	adopt_scope(action, "mPreambles");
}

/// @brief adopt_action_as_preabmel
/// Adds an action to the list of actions to run as the increment
/// Will take memory ownership of the pointer
/// @param action - Pointer to the action to adopt
void ActionListWhile::adopt_action_as_increment(Action* action)
{
	adopt_scope(action, "mIncrements");
}

/// @brief Update()
/// Step 1: Calls update on all preambles
/// Step 2: Checks if condition is greater than 0
/// Step 3: Calls update on all action in actionlist
/// Step 4: Calls update on all increments. Return to step 2
/// @param deltaTime - Time since last frame
void ActionListWhile::Update(const GameTime& deltaTime)
{
	// 1. Run preamble(s)
	{
		Datum* preambleDatum = get_datum_by_key("mPreambles");
		size_t sizeOfDatum = preambleDatum->size();
		for (size_t i = 0; i < sizeOfDatum; ++i)
		{
			Action* act = preambleDatum->get_scope_at(i)->As<Action>();
			assert(act != nullptr); // ERROR: Children of ActionListWhile was not an Action
			act->Update(deltaTime);
		}
	}
	// 2. check condition
	while (mCondition > 0)
	{
		// 3. Run body
		ActionList::Update(deltaTime);
		// 4. Increment and return to 2
		{
			Datum* incrementDatum = get_datum_by_key("mIncrements");
			size_t sizeOfDatum = incrementDatum->size();
			for (size_t i = 0; i < sizeOfDatum; ++i)
			{
				Action* act = incrementDatum->get_scope_at(i)->As<Action>();
				assert(act != nullptr); // ERROR: Children of ActionListWhile was not an Action
				act->Update(deltaTime);
			}
		}
	}
}


#pragma region InheritedFromAttributed
/// @brief Forwarding constructor
/// Passes along child's id to Attributed
/// @param idOfChild - id of child class
ActionListWhile::ActionListWhile(size_t idOfChild) : ActionList(idOfChild), mCondition(0)
{
}

/// @brief Clone()
/// @return Returns a deep copy of this
ActionListWhile* ActionListWhile::Clone() const
{
	return new ActionListWhile(*this);
}

/// @brief get_signatures
/// @return - Returns the signatures of this class's prescribed attributes
std::vector<Signature> ActionListWhile::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mPreambles", DatumType::Table, 0, 0 },
		{ "mIncrements", DatumType::Table, 0, 0 },
		{ "mCondition", DatumType::Integer, 1, offsetof(ActionListWhile, mCondition) },
	};
}
#pragma endregion