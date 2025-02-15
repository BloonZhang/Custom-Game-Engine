#include "pch.h"
#include "ActionList.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(ActionList);

/// @brief Default constructor
/// Calls Attributed typeID constructor
ActionList::ActionList() : Action(ActionList::TypeIdClass())
{
}

/// @brief get_list_of_actions_datum
/// @return Returns pointer to the Datum associated with the list of actions
Datum* ActionList::get_list_of_actions_datum()
{
	Datum* result = get_datum_by_key("mListOfActions");
	assert(result != nullptr); // ERROR: Could not find actions Datum
	return result;}

/// @brief add_action_to_list
/// Adds an action to the list. Will take over memory management for that pointer
/// @param action Pointer to Action to add to list
void ActionList::add_action_to_list(Action* action)
{
	adopt_scope(action, "mListOfActions");
}

/// @brief Update
/// Calls update on each of its Action children
/// @param deltaTime - time since last frame
void ActionList::Update(const GameTime& deltaTime)
{
	Datum* listOfActions = get_list_of_actions_datum();
	size_t numberOfActions = listOfActions->size();
	for (size_t i = 0; i < numberOfActions; ++i)
	{
		Action* action = listOfActions->get_scope_at(i)->As<Action>();
		assert(action != nullptr); // ERROR: Children of Action was not a Action
		action->Update(deltaTime);
	}
}

/// @brief Forwarding constructor
/// Passes along child's id to Attributed
/// @param idOfChild - id of child class
ActionList::ActionList(size_t idOfChild): Action(idOfChild)
{
}

/// @brief get_signatures
/// @return - Returns the signatures of this class's prescribed attributes
std::vector<Signature> ActionList::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mListOfActions", DatumType::Table, 0, 0 }
	};
}
