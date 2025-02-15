#pragma once

// .h file
/*
#include "Definitions.h"
#include "PARENT_NAME.h"

namespace Fiea::GameEngine
{
	class CLASS_NAME : public PARENT_NAME
	{
		RTTI_DECLARATIONS(CLASS_NAME, PARENT_NAME);

	public:
		CLASS_NAME();

		// Inherited from Attributed
	protected:
		CLASS_NAME(size_t);
	public:
		virtual CLASS_NAME* Clone() const override;
		static std::vector<Signature> get_signatures();
	private:
	};
}
*/

// .cpp file
/*
#include "pch.h"
#include "CLASS_NAME.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(CLASS_NAME);

/// @brief Default constructor
/// Calls Attributed typeID constructor
CLASS_NAME::CLASS_NAME() : PARENT_NAME(CLASS_NAME::TypeIdClass())
{
}

#pragma region InheritedFromAttributed
/// @brief Forwarding constructor
/// Passes along child's id to Attributed
/// @param idOfChild - id of child class
CLASS_NAME::CLASS_NAME(size_t idOfChild) : PARENT_NAME(idOfChild)
{
}

/// @brief Clone()
/// @return Returns a deep copy of this
CLASS_NAME* CLASS_NAME::Clone() const
{
	return new CLASS_NAME(*this);
}

/// @brief get_signatures
/// @return - Returns the signatures of this class's prescribed attributes
std::vector<Signature> CLASS_NAME::get_signatures()
{
	return std::vector<Signature>
	{

	};
}
#pragma endregion
*/