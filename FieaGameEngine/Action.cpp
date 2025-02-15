#include "pch.h"
#include "Action.h"


using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Action);

#pragma region InheritedFromAttributed
/// @brief Default constructor
/// Calls Attributed typeID constructor
Action::Action() : Attributed(Action::TypeIdClass()), mName("DefaultAttributed")
{
}

/// @brief Forwarding constructor
/// Passes along child's id to Attributed
/// @param idOfChild - id of child class
Action::Action(size_t idOfChild) : Attributed(idOfChild), mName("DefaultAttributedChild")
{
}

/// @brief get_signatures
/// @return - Returns the signatures of this class's prescribed attributes
std::vector<Signature> Fiea::GameEngine::Action::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mName", DatumType::String, 1, offsetof(Action, mName) }
	};
}
#pragma endregion

/// @brief name
/// @return Returns name of the action
const std::string Action::name() const
{
	return mName;
}

/// @brief setName
/// @param newName - string to set the new name to
void Action::setName(const std::string& newName)
{
	mName = newName;
}




