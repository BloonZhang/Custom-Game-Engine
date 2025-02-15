#pragma once
#include <string>
#include <stdexcept>

#include "Definitions.h"

namespace Fiea::GameEngine
{
	// Forward declaration for other classes
	//class Scope;
	//using ScopePtr = Scope*;
	//class RTTI;
	//using RTTIPtr = RTTI*;


	// public helper methods - strings


	// Class that holds multiple elements of a single type.
	// Type is determined at runtime
	class Datum
	{
	// --------------------- Typedefs ---------------------
	//public:
		//using ScopePtr = Scope*;
		//using RTTIPtr = RTTI*;

	// --------------- Forward declarations ---------------
	private:
		struct DatumNode;

	// --------------- public API functions ---------------
	public:
		// Default Constructor
		Datum();
		// Rule of 5: We are using allocated memory
		// Destructor
		~Datum();
		// Copy constructor
		Datum(const Datum&);
		// Move constructor
		Datum(Datum&&) noexcept;
		// assignment operator
		Datum& operator=(const Datum&);
		// move operator
		Datum& operator=(Datum&&) noexcept;

		// Converting constructor for each of the data types
		// TODO: initializer lists and {}
		Datum(int);
		Datum(float);
		Datum(const glm::vec4&);
		Datum(const glm::mat4&);
		Datum(const std::string&);
		// assignment operator overloads for scalars
		Datum& operator=(int);
		Datum& operator=(float);
		Datum& operator=(const glm::vec4&);
		Datum& operator=(const glm::mat4&);
		Datum& operator=(const std::string&);

		// Element access by index
		// Const Getters
		// TODO: how do I do it for operator[]?
		const int& get_int_at(size_t) const;
		const float& get_float_at(size_t) const;
		const glm::vec4& get_vec4_at(size_t) const;
		const glm::mat4& get_mat4_at(size_t) const;
		const std::string& get_string_at(size_t) const;
		// Mutable Getters
		// TODO: how do I do it for operator[]?
		int& get_int_at(size_t);
		float& get_float_at(size_t);
		glm::vec4& get_vec4_at(size_t);
		glm::mat4& get_mat4_at(size_t);
		std::string& get_string_at(size_t);
		// Const Getters by string
		const std::string get_int_at_as_string(size_t) const;
		const std::string get_float_at_as_string(size_t) const;
		const std::string get_vec4_at_as_string(size_t) const;
		const std::string get_mat4_at_as_string(size_t) const;
		//const std::string& get_string_at_as_string(size_t) const; // Unnecessary

		// Setters by copy. Returns reference to set element
		int& set_int_at(size_t, int);
		float& set_float_at(size_t, float);
		glm::vec4& set_vec4_at(size_t, const glm::vec4&);
		glm::mat4& set_mat4_at(size_t, const glm::mat4&);
		std::string& set_string_at(size_t, const std::string&);
		// Setters by string
		int& set_int_at_using_string(size_t, const std::string&);
		float& set_float_at_using_string(size_t, const std::string&);
		glm::vec4& set_vec4_at_using_string(size_t, const std::string&);
		glm::mat4& set_mat4_at_using_string(size_t, const std::string&);
		// std::string& set_string_at_using_string(size_t, const std::string&); // Unnecessary

		// External storage setters
		/* THE IDEA:
		A Datum can hold external storage instead of internal storage.
		If the Datum is set as external, it can never be set as internal again, and will only accept external storage.
		When holding external storage, it can never resize or remove memory, only modify individual elements
		The pointer to the external storage will be held in the data of a single DatumNode
		*/
		void set_external_int_storage(int*, size_t);
		void set_external_float_storage(float*, size_t);
		void set_external_vec4_storage(glm::vec4*, size_t);
		void set_external_mat4_storage(glm::mat4*, size_t);
		void set_external_string_storage(std::string*, size_t);
		void set_external_void_storage(void*, size_t, DatumType);

		// Size
		bool is_empty() const;
		size_t size() const;
		void set_size(size_t);

		// Data types
		void set_type(DatumType);
		DatumType get_type() const;
		bool is_unknown() const;
		bool is_compatible_with(DatumType) const;
		bool is_external() const;

		// Adding elements
		// Adding elements by copy
		void push_int_front(int);
		void push_float_front(float);
		void push_vec4_front(const glm::vec4&);
		void push_mat4_front(const glm::mat4&);
		void push_string_front(const std::string&);
		void push_int_back(int);
		void push_float_back(float);
		void push_vec4_back(const glm::vec4&);
		void push_mat4_back(const glm::mat4&);
		void push_string_back(const std::string&);
		// Adding elements by string
		void push_int_front(const std::string&);
		void push_float_front(const std::string&);
		void push_vec4_front(const std::string&);
		void push_mat4_front(const std::string&);
		//void push_string_front(const std::string&); // handled by push_string_back by copy
		void push_int_back(const std::string&);
		void push_float_back(const std::string&);
		void push_vec4_back(const std::string&);
		void push_mat4_back(const std::string&);
		//void push_string_back(const std::string&); // handled by push_string_back by copy
		/*
		// Inserting elements not required
		void insert_int_at(size_t, int);
		void insert_float_at(size_t, float);
		void insert_vec4_at(size_t, const glm::vec4&);
		void insert_mat4_at(size_t, const glm::mat4&);
		void insert_string_at(size_t, const std::string&);
		*/

		// Removing elements
		void clear();
		void pop_front();
		void pop_back();
		// Erasing elements not required
		// void erase_at(size_t);

		// Equality operators for Datums
		bool operator==(const Datum&) const;
		bool operator!=(const Datum&) const;
		// Equality operators for scalars
		bool operator==(int) const;
		bool operator!=(int) const;
		bool operator==(float) const;
		bool operator!=(float) const;
		bool operator==(const glm::vec4&) const;
		bool operator!=(const glm::vec4&) const;
		bool operator==(const glm::mat4&) const;
		bool operator!=(const glm::mat4&) const;
		bool operator==(const std::string&) const;
		bool operator!=(const std::string&) const;
		// Equality operators for scalars but the other way around
		friend bool operator==(int, const Datum&);
		friend bool operator!=(int, const Datum&);
		friend bool operator==(float, const Datum&);
		friend bool operator!=(float, const Datum&);
		friend bool operator==(const glm::vec4&, const Datum&);
		friend bool operator!=(const glm::vec4&, const Datum&);
		friend bool operator==(const glm::mat4&, const Datum&);
		friend bool operator!=(const glm::mat4&, const Datum&);
		friend bool operator==(const std::string&, const Datum&);
		friend bool operator!=(const std::string&, const Datum&);

		// Scope
		Datum(const ConstScopePtr&);
		ScopePtr& set_scope_at(size_t, const ConstScopePtr&);
		Datum& operator=(const ConstScopePtr&);
		const ConstScopePtr& get_scope_at(size_t) const;
		ScopePtr& get_scope_at(size_t);
		Scope& operator[](size_t);
		const Scope& operator[](size_t) const;
		void push_scope_front(const ConstScopePtr&);
		void push_scope_back(const ConstScopePtr&);
		bool find_scope(const ConstScopePtr&, size_t*) const;
		void remove_scope_at(size_t);

		// RTTI
		Datum(const RTTIPtr&);
		RTTIPtr& set_RTTI_at(size_t, const RTTIPtr&);
		Datum& operator=(const RTTIPtr&);
		const RTTIPtr& get_RTTI_at(size_t) const;
		RTTIPtr& get_RTTI_at(size_t);
		void push_RTTI_front(const RTTIPtr&);
		void push_RTTI_back(const RTTIPtr&);
		void set_external_RTTI_storage(RTTIPtr*, size_t);

	// ----------------- private members ------------------
	private:
		// Holds the type that this Datum stores
		DatumType mType;
		// Pointer to first and last node of data
		DatumNode* headNode;
		DatumNode* lastNode;
		// Number of elements this Datum currently has
		size_t mSize;
		// bool to indicate whether current storage is internal or external
		bool isExternal = false;
		
	// ------- implementation of forward declaration -------
	private:
		// Linked list implementation for underlying data storage
		// Each node has a pointer to some amount of memory corresponding with the Datum's mSizeOfType
		// Note: since this is a private struct within Datum.h, it cannot be directly accessed outside the class
		/* THE IDEA:
		Each Datum has a pointer to a linked list of DatumNodes.
		Each DatumNode has a pointer to one chunk of allocated memory, represented by a void*.
		The exact amount of allocated memory per DatumNode is determined by Datum's mSizeOfType.
		Datum does not actually know what kind of data it holds. It only knows what DatumType it holds. 
			i.e. it knows it holds DatumType::Integer, but it doesn't necessarily know that it's holding ints
		That means, the data held in data is simply pointers to bytes in memory. It will only return void*s.
		Therefore, it is up to the outside class to know what to do with the data returned by Datum.
		*/
		struct DatumNode
		{
			void* nodeData;
			DatumNode* next;
		};
		// Helper methods - DatumNodes
		// Creates a new DatumNode with a pointer to an amount of memory
		DatumNode* CreateNode(size_t);
		// Creates a new DatumNode either before or after a specified DatumNode
		DatumNode* CreateNodeBefore(DatumNode*, size_t);
		DatumNode* CreateNodeAfter(DatumNode*, size_t);
		// Creates a node used to hold external memory
		DatumNode* CreateExternalNode();
		// Gets the size of each DatumType
		size_t GetSizeOfDatumType(DatumType) const;
		// Traverses a linked list by a certain amount
		DatumNode* TraverseLinkedListBy(DatumNode*, size_t) const;
		// Copy data from one node to another
		void CopyDataFromToNode(DatumNode*, DatumNode*);
		// Checks if data in two nodes are the same
		bool NodesAreEqual(DatumNode*, DatumNode*) const;
		// Destructs and frees the memory held by a single node, and deletes the node itself
		void FreeNode(DatumNode*);
		// Destructs and frees an external node
		void FreeExternalNode(DatumNode*);
	};
}