#include "pch.h"
#include "Attributed.h"
#include "TypeRegistrySingleton.hpp"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Attributed);

#pragma region ConstructorsAndRuleOfFive
/// @brief Default constructor
/// Creates a special attribute in the Scope that is just the object of this object
/// @param typeId: the typeId of the highest level derived class
/// @param addressOfThis: the this of the highest level derived class
Attributed::Attributed(size_t typeId) : mTypeId(typeId)
{
	assert(this->size() == 0);
	// Place first element in scope tree,the address of this object itself
	Datum* rttiDatumPtr = this->add_datum("this");
	rttiDatumPtr->push_RTTI_back((RTTIPtr)this);
	// Then go through signatures and add those to scope/datum tree
	add_members_to_scope_tree(typeId);
}

/// @brief Copy Constructor
/// Copies over Scope tree, then re-does the prescribed attributes
/// @param rhs: the Attributed to copy
Attributed::Attributed(const Attributed& rhs) : Scope(rhs), mTypeId(rhs.mTypeId)
{
	// Called copy constructor of scope. We now have copies of the scope tree.
	// but we still need to correct the Datums that are external, as well as the one that is holding RTTI "this"
	// Place first element in scope tree,the address of this object itself
	Datum* rttiDatumPtr = this->add_datum("this");
	rttiDatumPtr->set_RTTI_at(0,((RTTIPtr)this));
	// Then go through signatures and add those to scope/datum tree
	add_members_to_scope_tree(mTypeId);
}

/// @brief Move Constructor
/// Shallow copies scope tree. Then re-does the prescribed attributes
/// @param rhs: the Attributed to copy
Attributed::Attributed(Attributed && rhs) noexcept : Scope(std::move(rhs)), mTypeId(std::move(rhs.mTypeId))
{
	// Called copy constructor of scope. We now have copies of the scope tree.
	// but we still need to correct the Datums that are external, as well as the one that is holding RTTI "this"
	// Place first element in scope tree,the address of this object itself
	Datum* rttiDatumPtr = this->add_datum("this");
	rttiDatumPtr->set_RTTI_at(0,((RTTIPtr)this));
	// Then go through signatures and add those to scope/datum tree
	add_members_to_scope_tree(mTypeId);
}

/// @brief Assignment operator
/// Calls Scope's operator=. Then re-does the prescribed attributes
/// @param rhs: the Attributed to copy
/// @return Returns *this
Attributed& Attributed::operator=(const Attributed& rhs)
{
	// Check for self-assignment
	if (this == &rhs) { return *this; }
	// call scope's operator=
	this->Scope::operator=(rhs);
	// copy over member variables of Attributed
	mTypeId = rhs.mTypeId;
	// redo the prescribed datums
	Datum* rttiDatumPtr = this->add_datum("this");
	rttiDatumPtr->set_RTTI_at(0,((RTTIPtr)this));
	add_members_to_scope_tree(mTypeId);
	return *this;
}

/// @brief Move assignment operator
/// Calls Scope's move operator=. Then re-does the prescribed attributes
/// @param rhs: the Attributed to copy
/// @return returns *this 
Attributed & Attributed::operator=(Attributed && rhs)
{
	// Check for self-assignment
	if (this == &rhs) { return *this; }
	// call scope's operator=
	this->Scope::operator=(std::move(rhs));
	// copy over member variables of Attributed
	mTypeId = rhs.mTypeId;
	// redo the prescribed datums
	Datum* rttiDatumPtr = this->add_datum("this");
	rttiDatumPtr->set_RTTI_at(0,((RTTIPtr)this));
	add_members_to_scope_tree(mTypeId);
	return *this;
}
#pragma endregion

#pragma region ScopeDatumTree
/// @brief add_members_to_scope_tree
/// helper function for inherited classes to create a scope/datum tree.
/// First, adds a datum "this" with a pointer to itself 
/// Then, checks the TypeRegistry depending on the given IdType, and adds Datums corresponding to the vector<Singature>
/// @param  typeId: the id of the class to add to the scope tree
/// @param locationOfThis: the location of the current this pointer in memory. usually just (size_t)this
void Attributed::add_members_to_scope_tree(IdType typeId)
{
	// Then go through signatures and add those to scope/datum tree
	std::vector<Signature> listOfSignatures = TypeRegistrySingleton::Instance->get_signatures_of_class((size_t)typeId);
	for (auto it : listOfSignatures)
	{
		Datum* newDatumPtr = this->add_datum(it.name);

		// if offset is 0, then default-constructed internal memory
		if (it.offset == 0)
		{
			newDatumPtr->set_type(it.type);
			newDatumPtr->set_size(it.sizeOfArray);
		}
		// Else, set external memory. two-way binded to the member variable
		else
		{
			size_t addressOfMemberVar = (size_t)this + (ptrdiff_t)it.offset;
			newDatumPtr->set_external_void_storage((void*)addressOfMemberVar, it.sizeOfArray, it.type);
		}
	}
}
/// @brief add_auxiliary_attribute
/// Adds an auxiliary attribute (i.e. a Datum) to this instance with the given name.
/// Throws an exception if the given key matches a prescribed attribute
/// @param key: The name of the auxiliar attribute to add
Datum* Attributed::add_auxiliary_attribute(std::string key)
{
	if (is_prescribed_attribute(key)) { throw std::invalid_argument("Tried to add auxiliary attribute with a key that matches a prescribed attribute."); }
	return this->add_datum(key);
}
#pragma endregion

#pragma region AttributeCheckers
/// @brief is_attribute
/// Predicate that checks if a given string matches the name of an attribute for this instance
/// @param key: the string to check if it matches the name of an attribute
/// @return Returns true if there is an attribute whose name matches the string. False otherwise
bool Attributed::is_attribute(std::string key) const
{
	return !(this->get_datum_by_key(key) == nullptr);
}
/// @brief is_prescribed_attribute
/// Predicate that checks if a given string matches the name of a prescribed attribute (i.e. an attribute for this class)
/// @param key: the string to check if it matches the name of a prescribed attribute
/// @return returns true if there is a prescribed attribute whose name matches the string. False otherwise
bool Attributed::is_prescribed_attribute(std::string key) const
{
	// If myTypeId is 0, then we did something wrong. This is calling on an actual Attributed, not a class that inherits from Attributed
	if (mTypeId == 0) { throw std::invalid_argument("Tried to call is_prescribed_attributed on an uninherited Attributed instance. How did you even get an instance of Attributed?"); }
	// Edge case: all classes have "this" as a prescribed attribute
	if (key == "this") { return true; }
	// Otherwise, go to type registry and get the signature
	std::vector<Signature> mySignatures = TypeRegistrySingleton::Instance->get_signatures_of_class(mTypeId);
	// For each signature, look for a match for key. std::find returns end() if not found
	auto predicate = [&key](Signature sig) -> bool { return sig.name == key; };
	return !(std::find_if(mySignatures.begin(), mySignatures.end(), predicate) == mySignatures.end());
}
/// @brief is_auxiliary_attribute
/// Predicate that checks if a given string matches the name of an auxiliary attribute (i.e. an attribute that was appended to this instance after construction)
/// @param key: the string to check if it matches the name of an auxiliary attribute
/// @return Returns true if there is an auxiliary attribute whose name matches the string. False otherwise
bool Attributed::is_auxiliary_attribute(std::string key) const
{
	return is_attribute(key) && !is_prescribed_attribute(key);
}
#pragma endregion

// TODO: delete
Attributed* Fiea::GameEngine::Attributed::Clone() const
{
	return nullptr;
}