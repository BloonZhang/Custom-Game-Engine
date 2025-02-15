#include "pch.h"
#include "Scope.h"
#include "RTTI.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Scope);

// ----------------------------------------------
// --------------- Scope functions --------------
// ----------------------------------------------
#pragma region DefaultAndRuleOfFive
/// @brief Default constructor
/// Does not allocate any memory
Scope::Scope() : mMap(), mVec(), mSize(0), mParent(nullptr)
{
}

/// @brief Destructor
/// First goes through and recursively deletes all of its children Scopes
/// Next, orphans itself by removing it from the Datum that stores it
/// Finally, deletes any Datums that it is holding
Scope::~Scope()
{
	// kill kids
	DestructChildrenRecursively();
	// tell parent that I'm dead
	OrphanSelf();
	// Destroy all datums inside this scope
	DestroyDatums();
}

/// @brief Copy constructor
/// Creates a deep copy of another Scope.
/// Uses the other Scope's key names to create deep copies of all Datums and Scopes
/// @param rhs: the Scope to copy
Scope::Scope(const Scope& rhs) : mMap(), mVec(), mSize(0), mParent(nullptr)
{
	// MIL: None
	// Note: we are not copying over mParent
	// deep copy datums
	// Note: code copied from operator=
	for (size_t index = 0; index < rhs.mSize; ++index)
	{
		// Get rhs's data
		const std::string& rhsKey = rhs.mVec[index];
		Datum* rhsDatumPtr = rhs.mMap.at(rhs.mVec[index]);
		// Create a new datum
		Datum* newDatumPtr = add_datum(rhsKey);
		// Copy over the datum
		//(*newDatumPtr) = (*rhsDatumPtr);
		
		// If the datum is not a DatumType::Table, we are safe to use Datum's operator=
		if (rhsDatumPtr->get_type() != DatumType::Table) { (*newDatumPtr) = (*rhsDatumPtr); } // TODO: this seems extremely bad for external datums. Oh well
		// If it is indeed a DatumType::Table, then we need to deep copy the Scopes within
		else
		{
			// For each scope within the rhs datum, copy it into our datum
			for (size_t datumIndex = 0; datumIndex < rhsDatumPtr->size(); ++datumIndex)
			{
				Scope* newScope = add_empty_scope(rhsKey);
				*newScope = (*rhsDatumPtr)[datumIndex];
			}
		}
	}
}

/// @brief Move constructor
/// Creates a shallow copy of another scope.
/// Reparents all children inside to self
/// TODO: Not sure this doesn't leak memory. Need to test more extensively
/// @param rhs: the scope to copy
Scope::Scope(Scope && rhs) noexcept : mMap(std::move(rhs.mMap)), mVec(std::move(rhs.mVec)), mSize(std::move(rhs.mSize)), mParent(std::move(rhs.mParent))
{
	// MIL: Moved all variables over, including mParent
	// reset everything in rhs
	rhs.mSize = 0;
	rhs.mParent = nullptr;
	// then make sure children know about the parent swap
	ResetParentOfChildren();
}

/// @brief Assignment operator
/// Deletes all memory allocated in this Scope, then creates a deep copy of another Scope.
/// Uses the other Scope's key names to create deep copies of all Datums and Scopes
/// However, WILL keep the same mParent. Does not copy the mParent of the other Scope
/// @param rhs: the other Scope to copy
/// @return Returns *this
Scope& Scope::operator=(const Scope& rhs)
{
	// TODO: should this error checking return gracefully or throw?
	// Make sure not self assigning
	if (&rhs == this) { return *this; }
	// Make sure not doing something really cursed, because this will cause me to destruct rhs
	if (rhs.IsDescendantOf(this)) { return *this; }
	// Make sure not doing something really cursed, because this will cause me to copy myself
	if (rhs.IsAncestorOf(this)) { return *this; }

	// First destruct all data EXCEPT parentage
	// kill kids
	DestructChildrenRecursively();
	// Destroy all datums inside this scope
	DestroyDatums();
	// Then set myself equal to rhs.
	// deep copy datums
	// Note: code copied to operator=
	for (size_t index = 0; index < rhs.mSize; ++index)
	{
		// Get rhs's data
		const std::string& rhsKey = rhs.mVec[index];
		Datum* rhsDatumPtr = rhs.mMap.at(rhs.mVec[index]);
		// Create a new datum
		Datum* newDatumPtr = add_datum(rhsKey);
		// If the datum is not a DatumType::Table, we are safe to use Datum's operator=
		if (rhsDatumPtr->get_type() != DatumType::Table) { (*newDatumPtr) = (*rhsDatumPtr); } // TODO: this seems extremely bad for external datums. Oh well
		// If it is indeed a DatumType::Table, then we need to deep copy the Scopes within
		else
		{
			// For each scope within the rhs datum, copy it into our datum
			for (size_t datumIndex = 0; datumIndex < rhsDatumPtr->size(); ++datumIndex)
			{
				// Note that this is calling operator= within operator=. Recursion
				Scope* newScope = add_empty_scope(rhsKey);
				*newScope = (*rhsDatumPtr)[datumIndex];
			}
		}
	}
	return *this;
}
/// @brief Move operator
/// Deletes all memory allocated in this Scope, then creates a shallow copy of another Scope.
/// Reparents all children to self
/// TODO: Not sure this doesn't leak memory. Need to test more extensively
/// @param rhs: the other Scope to copy
/// @return Returns *this
Scope& Scope::operator=(Scope&& rhs) noexcept
{
	// TODO: should this error checking return gracefully or throw?
	// Make sure not self assigning
	if (&rhs == this) { return *this; }
	// Make sure not doing something really cursed, because this will cause me to destruct rhs
	if (rhs.IsDescendantOf(this)) { return *this; }
	// Make sure not doing something really cursed, because this will cause me to copy myself
	if (rhs.IsAncestorOf(this)) { return *this; }

	// First destruct all data EXCEPT parentage
	// kill kids
	DestructChildrenRecursively();
	// Destroy all datums inside this scope
	DestroyDatums();

	mMap = std::move(rhs.mMap);
	mVec = std::move(rhs.mVec);
	mSize = std::move(rhs.mSize);
	mParent = std::move(rhs.mParent);
	// reset everything in rhs
	rhs.mSize = 0;
	rhs.OrphanSelf();
	// then make sure children know about the parent swap
	ResetParentOfChildren();

	return *this;
}
#pragma endregion

#pragma region CustomConstructors
/// @brief Constructor with inital capacity
/// A constructor that reserves a certain amount of initial capacity
/// Besides that, creates an empty Scope
/// @param initialCapacity: the amount of initial capacity to reserve
Scope::Scope(size_t initialCapacity): mMap(), mVec(), mSize(0), mParent(nullptr)
{
	mMap.reserve(initialCapacity);
	mVec.reserve(initialCapacity);
}
#pragma endregion

#pragma region Getters
/// @brief get_datum_by_key
/// Gets a Datum within this Scope that has a matching key.
/// @param key: the key to look for
/// @return Returns pointer to a Datum that matches, or nullptr if not found.
Datum* Scope::get_datum_by_key(const std::string& key)
{
	// check if has datum already
	if (mMap.find(key) != mMap.end()) { return mMap.at(key); }
	// else, we can't find it so return nullptr
	else { return nullptr; }
}

/// @brief get_datum_by_key const
/// Gets a Datum within this Scope that has a matching key.
/// @param key: the key to look for
/// @return Returns pointer to a const Datum that matches, or nullptr if not found.
const Datum* Scope::get_datum_by_key(const std::string& key) const
{
	// check if has datum already
	if (mMap.find(key) != mMap.end()) { return mMap.at(key); }
	// else, we can't find it so return nullptr
	else { return nullptr; }
}

/// @brief get_datum_in_parent_by_key
/// Gets a Datum within this Scope, or any ancestor Scope, that has a matching key.
/// @param key: the key to look for
/// @param scopeFoundAt: output pointer, will be set to the Scope that the datum is found in
/// @return Returns pointer to a Datum that matches, or nullptr if not found.
Datum* Scope::get_datum_in_parent_by_key(const std::string& key, ScopePtr* scopeFoundAt)
{
	// Assume that datum will be found in this scope. scopeFoundAt is this, and return pointer to Datum
	if (scopeFoundAt != nullptr) { *scopeFoundAt = this; }
	Datum* foundDatum = get_datum_by_key(key);
	if (foundDatum != nullptr) { return foundDatum; }
	// At this point, datum was not found in this scope. 
	else
	{
		// If we still have a parent, search them instead
		if (mParent != nullptr) { return mParent->get_datum_in_parent_by_key(key, scopeFoundAt); }
		// if parent is nullptr and we still haven't found it, too bad. We failed to find
		else { return nullptr; }
	}
}

/// @brief get_datum_in_parent_by_key const
/// Gets a Datum within this Scope, or any ancestor Scope, that has a matching key.
/// @param key: the key to look for
/// @param scopeFoundAt: output param, will be set to the Scope that the datum is found in
/// @return Returns pointer to a const Datum that matches, or nullptr if not found.
const Datum* Scope::get_datum_in_parent_by_key(const std::string& key, ConstScopePtr* scopeFoundAt) const
{
	// Assume that datum will be found in this scope. scopeFoundAt is this, and return pointer to Datum
	if (scopeFoundAt != nullptr) { *scopeFoundAt = this; }
	const Datum* foundDatum = get_datum_by_key(key);
	if (foundDatum != nullptr) { return foundDatum; }
	// At this point, datum was not found in this scope. 
	else
	{
		// If we still have a parent, search them instead
		if (mParent != nullptr) { return mParent->get_datum_in_parent_by_key(key, scopeFoundAt); }
		// if parent is nullptr and we still haven't found it, too bad. We failed to find
		else { return nullptr; }
	}
}

/// @brief get_datum_that_contains_scope
/// Get a Datum within this Scope that contains a specified Scope
/// @param scopePtrToFind: Pointer to the Scope that we are looking for
/// @param outIndex: output param, will be set to the index of the Datum that the Scope is found at
/// @return Returns pointer to a Datum that has the Scope
Datum* Scope::get_datum_that_contains_scope(const ConstScopePtr& scopePtrToFind, size_t* outIndex)
{
	// For each datum in this scope
	for (size_t index = 0; index < mSize; ++index)
	{
		Datum* datumPtr = mMap.at(mVec[index]);
		// Check that the Datum is Table
		if (datumPtr->get_type() != DatumType::Table) { continue; }
		// Within that datum, check for the existance of scopePtrToFind. continue if not found
		size_t indexFoundAt = 0_z;
		if (!datumPtr->find_scope(scopePtrToFind, &indexFoundAt)) { continue; }
		// at this point, indexFoundAt is the index of the scope. return that, and the datumptr
		*outIndex = indexFoundAt;
		return datumPtr;
	}
	return nullptr;
}

/// @brief get_datum_that_contains_scope const
/// Get a Datum within this Scope that contains a specified Scope
/// @param scopePtrToFind: Pointer to the Scope that we are looking for
/// @param outIndex: output param, will be set to the index of the Datum that the Scope is found at
/// @return Returns pointer to a const Datum that has the Scope
const Datum* Scope::get_datum_that_contains_scope(const ConstScopePtr& scopePtrToFind, size_t* outIndex) const
{
	// For each datum in this scope
	for (size_t index = 0; index < mSize; ++index)
	{
		const Datum* datumPtr = mMap.at(mVec[index]);
		// Check that the Datum is Table
		if (datumPtr->get_type() != DatumType::Table) { continue; }
		// Within that datum, check for the existance of scopePtrToFind. continue if not found
		size_t indexFoundAt = 0_z;
		if (!datumPtr->find_scope(scopePtrToFind, &indexFoundAt)) { continue; }
		// at this point, indexFoundAt is the index of the scope. return that, and the datumptr
		*outIndex = indexFoundAt;
		return datumPtr;
	}
	return nullptr;
}
#pragma endregion

#pragma region Adding
/// @brief add_datum
/// Given a key, either adds a Datum with that key to this Scope if no Datum already exists, or returns the existing Datum
/// @param key: the key associated with the Datum to add
/// @return Returns a pointer to a Datum that corresponds with the key
Datum* Scope::add_datum(const std::string& key)
{
	// check if has datum already
	if (mMap.find(key) != mMap.end())
	{
		// Do nothing if it already has the Datum
		return mMap.at(key);
	}
	// else, we'll have to make a new one
	else
	{
		// Make new datum at new key
		mMap[key] = new Datum();
		// Push this key onto the ordered vec
		mVec.push_back(key);
		// increase size
		++mSize;
		// return new Datum
		return mMap.at(key);
	}
}

/// @brief add_empty_scope
/// Creates a new empty Scope at the back of the Datum with the specified key.
/// If a Datum with the key does not exist, will create a new Datum with the key
/// @param key: the key associated with the Datum that will hold the new Scope
/// @return Returns a pointer to the new Scope
Scope* Scope::add_empty_scope(const std::string& key)
{
	// Get the datum with key, and if it doesn't exist then it will be created
	Datum* newDatum = add_datum(key);
	// Set datum's type to Table if it wasn't already
	if (newDatum->get_type() == DatumType::Unknown) { newDatum->set_type(DatumType::Table); }
	// If the datum's type is not unknown and not table, something went wrong
	else if (newDatum->get_type() != DatumType::Table) { throw std::invalid_argument("Tried to add_scope to a Datum with the wrong DatumType."); }
	// Append a new scope to that Datum. A bit inefficient, since if it was an empty datum we could have just directly called the scalar constructor but ¯\_(^v^)_/¯
	Scope* newScope = new Scope();
	newDatum->push_scope_back(newScope);
	// Set the parent of the new scope to this
	newScope->SetParent(this);
	// Return the pointer to the newly made scope
	return newScope;
}

/// @brief add_existing_scope
/// Places an existing scope at the back of the Datum with the specified key
/// Unsure if needed.
/// @param key 
/// @param existingScope 
/// @return 
/*
Scope* Scope::add_existing_scope(const std::string& key, ScopePtr existingScope)
{
	return nullptr;
}
*/
#pragma endregion

#pragma region ScopeTree
/// @brief adopt_scope
/// Will set a Scope as a child of this scope, removing it from its previous parent (if any)
/// @param scopeToAdopt: Pointer to the scope to adopt
/// @param key: Key to use for the Datum to store this adopted Scope
void Scope::adopt_scope(Scope* scopeToAdopt, const std::string& key)
{
	// Check for self adoption
	// TODO: should this return gracefully or throw an exception?
	if (scopeToAdopt == this) { throw std::invalid_argument("Self-adoption in Scope::adopt_scope"); return; }
	// Check for infinite circular dependency
	// TODO: should this return gracefully or throw an exception?
	if (this->IsDescendantOf(scopeToAdopt)) { throw std::invalid_argument("Circular dependency in Scope::adopt_scope"); return; }
	// Orphan the scope to adopt
	scopeToAdopt->OrphanSelf();
	// Set the new parent to sef
	scopeToAdopt->SetParent(this);
	// Get datum with key (creating if necessary)
	Datum* datumToAddTo = this->add_datum(key);
	// Add the adopted scope to the datum
	datumToAddTo->push_scope_back(scopeToAdopt);
}

/// @brief parent_datum_and_index
/// Gets the Datum that this Scope is currently held in, if any
/// @param indexFoundAt: output param, the index of the Datum that this Scope lives in
/// @return Returns a pointer to the Datum that stores this Scope
Datum* Scope::parent_datum_and_index(size_t* indexFoundAt)
{
	// If it has no parent
	if (mParent == nullptr) { return nullptr; }
	// Otherwise, it does have a parent, which means it lives inside a Datum
	Datum* myOwningDatumPtr = mParent->get_datum_that_contains_scope(this, indexFoundAt);
	assert(myOwningDatumPtr != nullptr); // if the find failed, something really bad happened with my scope tree
	return myOwningDatumPtr;
}
/// @brief parent_datum_and_index const
/// Gets the Datum that this Scope is currently held in, if any
/// @param indexFoundAt: output param, the index of the Datum that this Scope lives in
/// @return Returns a pointer to a const Datum that stores this Scope
const Datum* Scope::parent_datum_and_index(size_t* indexFoundAt) const
{
	// If it has no parent
	if (mParent == nullptr) { return nullptr; }
	// Otherwise, it does have a parent, which means it lives inside a Datum
	Datum* myOwningDatumPtr = mParent->get_datum_that_contains_scope(this, indexFoundAt);
	assert(myOwningDatumPtr != nullptr); // if the find failed, something really bad happened with my scope tree
	return myOwningDatumPtr;
}

/// @brief parent_scope
/// @return Returns a pointer to the Scope that is the parent of this Scope
Scope* Scope::parent_scope()
{
	return mParent;
}
/// @brief parent_scope
/// @return Returns a pointer to the Scope that is the parent of this Scope
Scope* Scope::parent_scope() const
{
	return mParent;
}
#pragma endregion

#pragma region OperatorSquareBrackets
/// @brief operator[string]
/// Accesses the Datum with the specified key, creating one if necessary
/// @param key: the key to search for
/// @return Returns a reference to the Datum with that key
Datum& Scope::operator[](const std::string& key)
{
	return *add_datum(key);
}
/// @brief operator[size_t]
/// Accesses the Datum at the specified index
/// Will throw an exception if index is greater than mSize
/// @param index: the index of the Datum to get
/// @return Returns a reference to the Datum at the index
Datum& Scope::operator[](size_t index)
{
	// Throw if out of range
	if (index >= mSize) { throw std::out_of_range("Index is out of range in operator[](size_t)"); }
	// Assert that the string in the vector is actually a key in the dictionary
	assert(mMap.find(mVec[index]) != mMap.end());
	return *mMap.at(mVec[index]);
}
// TODO: can I make a const version of operator[index]?
#pragma endregion

#pragma region EqualityOperators
/// @brief size
/// @return Returns the sie of the Datum
size_t Scope::size() const
{
	return mSize;
}

/// @brief operator==
/// Compares two scopes for equality in three ways:
// 1. The sizes of the two scopes are equal
// 2. The keys in the two scopes are the same strings
// 3. The datums of the two scopes are equal
//		3a. the type of the datums are equal
//		3b. the contents of the datum are equal
// Note: It's ok for two scopes to have different parents
/// @param rhs: the Scope to compare with
/// @return Returns true if the Scopes are equal in all three conditions
bool Scope::operator==(const Scope& rhs) const
{
	// If size is not equal
	if (this->mSize != rhs.mSize) { return false; }
	// If vectors are not equal (i.e. the keys are not equal)
	if (this->mVec != rhs.mVec) { return false; }
	// Proceed to compare mMap values, but manually since Datum does not know how to deep compare DatumType::Table
	for (size_t vecIndex = 0; vecIndex < mSize; ++vecIndex)
	{
		Datum* datumPtr = mMap.at(mVec[vecIndex]);
		Datum* rhsDatumPtr = rhs.mMap.at(rhs.mVec[vecIndex]);
		// first, compare that the Datums are the same type and size
		if (datumPtr->get_type() != rhsDatumPtr->get_type() || datumPtr->size() != rhsDatumPtr->size()) { return false; }
		// If the datum is not a DatumType::Table, then Datum can take care of comparing
		if (datumPtr->get_type() != DatumType::Table) 
		{ 
			if (*datumPtr != *rhsDatumPtr) { return false; } 
			else { continue; }
		}
		// Otherwise, we have to manually compare the two Datums
		for (size_t datumIndex = 0; datumIndex < datumPtr->size(); ++datumIndex)
		{
			// Compare the scope at the datum index. Note this calls Scope's operator==, so it is recursive
			if ((*datumPtr)[datumIndex] != (*rhsDatumPtr)[datumIndex]) { return false; }
		}
	}
	// At this point, we have confirmed:
	// 1. The sizes of the two scopes are equal
	// 2. The keys in the two scopes are the same strings
	// 3. The datums of the two scopes are equal
	//		3a. the type of the datums are equal
	//		3b. the contents of the datum are equal
	// Note: we are intentionally not comparing mParents. It's ok for two scopes to have different parents
	return true;
}

/// @brief operator!=
/// Compares two scopes for equality in three ways:
// 1. The sizes of the two scopes are equal
// 2. The keys in the two scopes are the same strings
// 3. The datums of the two scopes are equal
//		3a. the type of the datums are equal
//		3b. the contents of the datum are equal
// Note: It's ok for two scopes to have different parents
/// @param rhs: the Scope to compare with
/// @return Returns true if the Scopes are not equal in all three conditions
bool Scope::operator!=(const Scope& rhs) const
{
	return !this->operator==(rhs);
}
#pragma endregion

#pragma region Clone
/// @brief Clone
/// Virtual wrapper for copy constructor
/// @return Returns a pointer to a new Scope that was copy constructed from this Scope (nodiscard)
Scope* Scope::Clone() const
{
	return new Scope(*this);
}
#pragma endregion

// ----------------------------------------------
// ------------ Scope Helper functions-----------
// ----------------------------------------------
#pragma region Helpers
/// @brief SetParent
/// Sets the parent of a Scope to another parent
/// @param newParent: the new parent
void Scope::SetParent(Scope* newParent)
{
	mParent = newParent;
}

/// @brief DestroyDatums
/// Goes through each Datum owned by this Scope and deletes it, destructing it and freeing its memory.
/// Then, clears the map and vector, and sets size to 0
void Scope::DestroyDatums()
{
	// for each entry in the map
	for (std::pair<std::string, Datum*> iterator : mMap)
	{
		// Delete the datum
		delete iterator.second;
	}
	// clear map
	mMap.clear();
	// clear vector
	mVec.clear();
	// set size to zero
	mSize = 0;
}

/// @brief OrphanSelf
/// Removes this Scope from its parent Datum.
/// Does not destruct or free any memory.
void Scope::OrphanSelf()
{
	// If parent is nullptr, just ignore
	if (mParent == nullptr) { return; }
	// in the parent, look for the Datum that holds this
	size_t index = 0;
	/*
	Datum* myOwningDatumPtr = mParent->get_datum_that_contains(this, &index);
	assert(myOwningDatumPtr != nullptr); // if the find failed, something really bad happened with my scope tree
	*/
	Datum* myOwningDatumPtr = parent_datum_and_index(&index);
	// In the Datum that holds this, remove myself
	myOwningDatumPtr->remove_scope_at(index);
	// Set parent to nullptr
	mParent = nullptr;
}

/// @brief DestructChildrenRecursively
/// Goes through all immediate Scope children owned by this Scope, and deletes them
void Scope::DestructChildrenRecursively()
{
	// For each datum in this scope
	for (size_t index = 0; index < mSize; ++index)
	{
		Datum* datumPtr = mMap.at(mVec[index]);
		// Check that the Datum is Table
		if (datumPtr->get_type() != DatumType::Table) { continue; }
		// For each scope in the Datum
		//for (int datumIndex = 0; datumIndex < datumPtr->size(); ++datumIndex)
		while (datumPtr->size() > 0)
		{
			// Call the scope destructors
			// (*datumPtr)[datumIndex].~Scope(); // I kind of hate datum operator[] so I'll use the gross syntax
			delete datumPtr->get_scope_at(0);
		}
	}
}

/// @brief SetParentOfChildren
/// Goes through all immediate Scope children and reparents them to this
/// Called after move semantics
void Scope::ResetParentOfChildren()
{
	// For each datum in this scope
	for (size_t index = 0; index < mSize; ++index)
	{
		Datum* datumPtr = mMap.at(mVec[index]);
		// Check that the Datum is Table
		if (datumPtr->get_type() != DatumType::Table) { continue; }
		// For each scope in the Datum
		for (size_t datumIndex = 0; datumIndex < datumPtr->size(); ++datumIndex)
		{
			// Just set parent to this
			(*datumPtr)[datumIndex].SetParent(this);
		}
	}
}

/// @brief IsAncestorOf
/// Checks if this Scope is an ancestor of another scope
/// @param potentialDescendantScope: the other Scope to check
/// @return Returns true if this is an ancestor of the other Scope
bool Scope::IsAncestorOf(const ConstScopePtr& potentialDescendantScope) const
{
	return potentialDescendantScope->IsDescendantOf(this);
}
/// @brief IsDescendantOf
/// Checks if this Scope is a descendant of another Scope
/// @param potentialAncestorScope: the other Scope to check
/// @return Returns true if this is a descendant of the other Scope
bool Scope::IsDescendantOf(const ConstScopePtr& potentialAncestorScope) const
{
	// if potentialAncestorScope is this, then something really bad has happened
	assert(this != potentialAncestorScope);
	// if mParent is nullptr, then we know there's no ancestor
	if (mParent == nullptr) { return false; }
	// Otherwise, check if our parent matches the scope that's passed in
	if (mParent == potentialAncestorScope) { return true; }
	// Otherotherwise, go up the parent and check 
	else { return mParent->IsDescendantOf(potentialAncestorScope); }
}
#pragma endregion