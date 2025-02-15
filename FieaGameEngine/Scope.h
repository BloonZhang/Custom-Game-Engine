#pragma once

#include <vector>
#include <unordered_map>

#include "RTTI.h"
#include "Datum.h"

#include "Definitions.h"

/*
Scope Assignment Booster Knowledge!

All scopes have an ancestor that is a naked Scope (not contained in a Datum)
		AppendScope and Adopt are the only way we should be able to get a scope nested within another scope
		What does this mean for copy construction of a scope?What can you do with the Datum API to help guarantee this?
			hint: think  about Friending
	If a scope needs to own the memory for it's child scopes...
		What does that mean for copy construction?
			If I copy-construct a Scope, can I just use the same parent? (the answer is probably no, but think about WHY)
		What about copy assignment?
		What about destruction? 
		What if I destruct a scope that is currently nested? What might I need to do to keep my "tree" healthy?

Search
	Note that this API takes a Scope** in order to provide an "output parameter" of type Scope*... think about how this makes sense from what we talked about with pointers as parameters today!
	Why are we returning a Datum* instead of a Datum&?
	Could I take a Scope*& as my output parameter?
		The answer is yes, but if I chose to do this, how might I make that parameter optional?

Append/AppendScope
	Remember that Datum can hold arrays of things
	Note the behavior different between AppendScope and Append... AppendScope actually puts a new scope into the created Datum, while Append just makes sure that there is a Datum at that key and leaves it to the user to put something in it...
		This is part of the way that the API maintains ownership. If I only return the datum from AppendScope, then the user would have to add their own Scope to the Datum, which is something we really don't want ANYONE EXCEPT ANOTHER SCOPE to be able to do

FindContainedScope
	Note that there are two outputs... this means you'll either need to provide an output parameter for the index (similar to what we did with search) or use std::pair or std::tuple
		For situations with multiple returns, the standard currently favors a tuple-based implementation, but you may use either approach, as you see fit
		For search, we chose to explicitly use an output parameters given the consideration that the returned Scope* might be optionally requested... think about that. What would be the undesirable cost of returning a tuple, in that case?
*/


namespace Fiea::GameEngine {

	class Datum;

	class Scope : public RTTI {
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		Scope();
		virtual ~Scope() override;
		Scope(const Scope&);
		Scope(Scope&&) noexcept;
		Scope& operator=(const Scope&);
		Scope& operator=(Scope&&) noexcept;

		Scope(size_t);

		Datum* get_datum_by_key(const std::string&);
		const Datum* get_datum_by_key(const std::string&) const;
		Datum* get_datum_in_parent_by_key(const std::string&, ScopePtr*);
		const Datum* get_datum_in_parent_by_key(const std::string&, ConstScopePtr*) const;
		Datum* get_datum_that_contains_scope(const ConstScopePtr&, size_t*);
		const Datum* get_datum_that_contains_scope(const ConstScopePtr&, size_t*) const;
		Datum* add_datum(const std::string&);
		Scope* add_empty_scope(const std::string&);
		//Scope* add_existing_scope(const std::string&, ScopePtr);
		void adopt_scope(Scope*, const std::string&);
		Datum* parent_datum_and_index(size_t*);
		const Datum* parent_datum_and_index(size_t*) const;
		Scope* parent_scope();
		Scope* parent_scope() const;

		Datum& operator[](const std::string&);
		Datum& operator[](size_t);
		// TODO: const index getter? like const Datum& get_datum_at(size_t) const;
		// If I do this, then go through and replace all mMap.at(mVec[index])

		size_t size() const;
		bool operator==(const Scope&) const;
		bool operator!=(const Scope&) const;

		/*
		virtual std::string ToString() const;
		virtual bool Equals(const RTTI* rhs) const;
		*/

		[[nodiscard]] virtual Scope* Clone() const;

	private:
		void SetParent(Scope*);
		void DestroyDatums();
		void OrphanSelf();
		void DestructChildrenRecursively();
		void ResetParentOfChildren();
	//public: // TEMPORARILY PUBLIC for testing
		bool IsAncestorOf(const ConstScopePtr&) const;
		bool IsDescendantOf(const ConstScopePtr&) const;
		
	private:
		size_t mSize;
		// size_t mCapacity;
		ScopePtr mParent;
	private:
		/* THE IDEA:
		Each scope will have two data structures, one unordered map<string, Datum*> and one vector<string>.
		Since unordered maps do not invalidate pointers to the data upon resizing, the map will be the "owner" data holder.
			That is, the map will be in charge of allocating and freeing memory for Datums.
		The vector will then hold an ordered list of all those keys
		*/
		std::unordered_map<std::string, Datum*> mMap;
		std::vector<std::string> mVec;
	};
}