#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include <stdexcept>

namespace Fiea::GameEngine
{
	// ----------------------------------------------
	// ----------- String Helper functions ----------
	// ----------------------------------------------
#pragma region StringHelperFunctions

#pragma endregion

	// ----------------------------------------------
	// -------------- Friend functions --------------
	// ----------------------------------------------
#pragma region FriendFunctions
	bool operator==(int i, const Datum& datum) { return datum.operator==(i); }
	bool operator!=(int i, const Datum& datum) { return datum.operator!=(i); }
	bool operator==(float f, const Datum& datum) { return datum.operator==(f); }
	bool operator!=(float f, const Datum& datum) { return datum.operator!=(f); }
	bool operator==(const glm::vec4& vec, const Datum& datum) { return datum.operator==(vec); }
	bool operator!=(const glm::vec4& vec, const Datum& datum) { return datum.operator!=(vec); }
	bool operator==(const glm::mat4& mat, const Datum& datum) { return datum.operator==(mat); }
	bool operator!=(const glm::mat4& mat, const Datum& datum) { return datum.operator!=(mat); }
	bool operator==(const std::string& str, const Datum& datum) { return datum.operator==(str); }
	bool operator!=(const std::string& str, const Datum& datum) { return datum.operator!=(str); }
#pragma endregion

	// ----------------------------------------------
	// --------------- Datum functions --------------
	// ----------------------------------------------
#pragma region DefaultAndRuleOfFive
	/// @brief Default Constructor
	/// Sets mType to Unknown. Node pointers are nullptr.
	/// size starts at 0
	Datum::Datum() : mType(DatumType::Unknown), headNode(nullptr), lastNode(nullptr), mSize(0)
	{
	}

	/// @brief Destructor
	/// Goes through all nodes and destructs and frees the data they are pointing to, then frees the actual node itself
	Datum::~Datum()
	{
		clear();
	}

	/// @brief Copy Constructor
	/// Creates a Datum that is a deep copy of another Datum. Leaves the other Datum unchanged.
	/// @param rhs: the Datum to copy
	Datum::Datum(const Datum& rhs) : mType(rhs.mType), headNode(nullptr), lastNode(nullptr), mSize(rhs.mSize), isExternal(rhs.isExternal)
	{
		// If external
		if (isExternal)
		{
			switch (rhs.mType)
			{
			case DatumType::Integer:
				set_external_int_storage((int*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Float:
				set_external_float_storage((float*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Vector4:
				set_external_vec4_storage((glm::vec4*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Matrix4:
				set_external_mat4_storage((glm::mat4*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::String:
				set_external_string_storage((std::string*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Table:
				// TODO: currently external scope is unsupported. Add in the future?
				assert(false);
				break;
			case DatumType::Pointer:
				set_external_RTTI_storage((RTTIPtr*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Unknown:
				throw std::invalid_argument("Passed in Unknown to external Datum(Datum).");
				break;
			default:
				throw std::invalid_argument("Invalid argument passed into external Datum(Datum).");
				break;
			}
		}
		// If internal
		else
		{
			// Pointer for traversing rhs's data
			DatumNode* rhsPtr = rhs.headNode;
			// for each element, create a copy of rhs's element
			for (size_t i = 0; i < rhs.mSize; ++i)
			{
				// Create a new node to hold that element
				lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(rhs.mType));
				// If this was the first node, also go ahead and set headNode
				if (i == 0) { headNode = lastNode; }
				// Create a copy of the element at that index
				CopyDataFromToNode(rhsPtr, lastNode);
				// Increment pointer going through rhs
				rhsPtr = rhsPtr->next;
			}
		}
	}

	/// @brief Move Constructor
	/// Creates a datum that is a shallow copy of another Datum, then removes the pointers from the other Datum
	/// @param rhs: Datum to be moved from
	Datum::Datum(Datum&& rhs) noexcept : mType(rhs.mType), headNode(rhs.headNode), lastNode(rhs.lastNode), mSize(rhs.mSize), isExternal(rhs.isExternal)
	{
		// reset member variables of rhs
		rhs.mType = DatumType::Unknown;
		rhs.mSize = 0;
		// null the pointers owned by rhs
		rhs.headNode = nullptr;
		rhs.lastNode = nullptr;
	}

	/// @brief Assignment operator operator=(Datum)
	/// Deletes the data currently inside this Datum, and then creates a deep copy of another Datum
	/// Does not support switching DatumType. Will throw an exception if the DatumTypes do not match
	/// Does not support assignment between internal and external Datums
	/// @param rhs: Datum to be copied
	/// @return Returns a reference to the Datum itself
	Datum& Datum::operator=(const Datum& rhs)
	{
		// First check for self assignment
		if (this == &rhs) { return *this; }
		// If this Datum is not Unknown, then we also have to go through some checks
		if (this->mType != DatumType::Unknown)
		{
			// Also check if Datums do not agree in terms of internal versus external
			if (this->isExternal != rhs.isExternal) { throw std::invalid_argument("Internal/External mismatch in operator=(Datum)."); }
			// Also check if the types are mismatched
			if (this->mType != DatumType::Unknown && this->mType != rhs.mType) { throw std::invalid_argument("Type of Datum passed into operator=(Datum) does not match."); }
		}
		// For internal, deep copy all data
		if (rhs.isExternal == false)
		{
			// Delete old data
			clear();
			// Re-set all variables
			this->mSize = rhs.mSize;
			this->mType = rhs.mType;	// Despite this line, operator= does not support changing DatumType.
										// This is strictly for the case that this->mType is Unknown
			// Pointer for traversing rhs's data
			DatumNode* rhsPtr = rhs.headNode;
			// for each element, create a copy of rhs's element
			for (size_t i = 0; i < rhs.mSize; ++i)
			{
				// Create a new node to hold that element
				lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(rhs.mType));
				// If this was the first node, also go ahead and set headNode
				if (i == 0) { headNode = lastNode; }
				// Create a copy of the element at that index
				CopyDataFromToNode(rhsPtr, lastNode);
				// Increment pointer going through rhs
				rhsPtr = rhsPtr->next;
			}
		}
		// For external, shallow copy the one node
		else
		{
			switch (rhs.mType)
			{
			case DatumType::Integer:
				set_external_int_storage((int*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Float:
				set_external_float_storage((float*)rhs.headNode->nodeData, rhs.size());
			break;
			case DatumType::Vector4:
				set_external_vec4_storage((glm::vec4*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Matrix4:
				set_external_mat4_storage((glm::mat4*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::String:
				set_external_string_storage((std::string*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Table:
				// TODO: currently external scope is unsupported. Add in the future?
				assert(false);
				break;
			case DatumType::Pointer:
				set_external_RTTI_storage((RTTIPtr*)rhs.headNode->nodeData, rhs.size());
				break;
			case DatumType::Unknown:
				throw std::invalid_argument("Passed in Unknown to external operator=(Datum).");
				break;
			default:
				throw std::invalid_argument("Invalid argument passed into external operator=(Datum).");
				break;
			}
		}
		// Return this
		return *this;
	}


	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		// First check for self assignment
		if (this == &rhs) { return *this; }
		// If this Datum is not Unknown, then we also have to go through some checks
		// Note: we cannot throw because move operator is noexcept. returns self if invalid
		// TODO: how to notify outside code that the code was not executed?
		if (this->mType != DatumType::Unknown)
		{
			// Also check if Datums do not agree in terms of internal versus external
			if (this->isExternal != rhs.isExternal) 
			{ 
				assert(false);
				//throw std::invalid_argument("Internal/External mismatch in move operator=(Datum)."); 
				return *this;
			}
			// Also check if the types are mismatched
			if (this->mType != DatumType::Unknown && this->mType != rhs.mType) 
			{ 
				assert(false);
				//throw std::invalid_argument("Type of Datum passed into move operator=(Datum) does not match."); 
				return *this;
			}
		}
		// For internal AND external, shallow copy all data
		// Delete old data
		clear();
		// Shallow copy all data
		this->mSize = rhs.mSize;
		this->mType = rhs.mType;	// Despite this line, operator= does not support changing DatumType.
									// This is strictly for the case that this->mType is Unknown
		this->headNode = rhs.headNode;
		this->lastNode = rhs.lastNode;
		this->isExternal = rhs.isExternal;
		// reset member variables of rhs
		rhs.mType = DatumType::Unknown;
		rhs.mSize = 0;
		rhs.headNode = nullptr;
		rhs.lastNode = nullptr;
		// Return this
		return *this;
	}
#pragma endregion

#pragma region ScalarConstructorsAndOperator=
	/// @brief Converting constructor: int
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param i: int to copy into the Datum
	Datum::Datum(int i) : mType(DatumType::Integer), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) int(i);
	}

	/// @brief Converting constructor: float
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param f: float to copy into the Datum
	Datum::Datum(float f) : mType(DatumType::Float), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) float(f);
	}

	/// @brief Converting constructor: vec4
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param vec: vec4 to copy into the Datum
	Datum::Datum(const glm::vec4& vec) : mType(DatumType::Vector4), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) glm::vec4(vec);
	}

	/// @brief Converting constructor: mat4
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param mat: mat4 to copy into the Datum
	Datum::Datum(const glm::mat4& mat) : mType(DatumType::Matrix4), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) glm::mat4(mat);
	}

	/// @brief Converting constructor: string
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param str: string to copy into the Datum
	Datum::Datum(const std::string& str) : mType(DatumType::String), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) std::string(str);
	}

	/// @brief Scalar assignment operator=(int)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param i: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(int i)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(int) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Integer)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(int) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Integer;
		// Create a single node with a copy of the data
		push_int_back(i);
		// Return this
		return *this;
	}

	/// @brief Scalar assignment operator=(float)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param f: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(float f)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(float) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Float)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(float) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Float;
		// Create a single node with a copy of the data
		push_float_back(f);
		// Return this
		return *this;
	}

	/// @brief Scalar assignment operator=(vec4)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param vec: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(const glm::vec4& vec)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(vec4) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Vector4)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(vec4) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Vector4;
		// Create a single node with a copy of the data
		push_vec4_back(vec);
		// Return this
		return *this;
	}

	/// @brief Scalar assignment operator=(mat4)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param mat: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(const glm::mat4& mat)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(mat4) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Matrix4)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(mat4) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Matrix4;
		// Create a single node with a copy of the data
		push_mat4_back(mat);
		// Return this
		return *this;
	}

	/// @brief Scalar assignment operator=(string)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param str: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(const std::string& str)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(string) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::String)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(str) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::String;
		// Create a single node with a copy of the data
		push_string_back(str);
		// Return this
		return *this;
	}
#pragma endregion

#pragma region Getters
	/// @brief get_int_at const
	/// Gets a const reference to the data at the specific index, cast as an int.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const int& Datum::get_int_at(size_t index) const
	{
		if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in get_int_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_int_at const."); }	
		if (isExternal) { return ((int*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(int*)nodePtr->nodeData;
	}

	/// @brief get_float_at const
	/// Gets a const reference to the data at the specific index, cast as a float.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const float& Datum::get_float_at(size_t index) const
	{
		if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in get_float_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_float_at const."); }
		if (isExternal) { return ((float*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(float*)nodePtr->nodeData;
	}

	/// @brief get_vec4_at const
	/// Gets a const reference to the data at the specific index, cast as a vec4.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const glm::vec4& Datum::get_vec4_at(size_t index) const
	{
		if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in get_vec4_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_vec4_at const."); }
		if (isExternal) { return ((glm::vec4*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(glm::vec4*)nodePtr->nodeData;
	}

	/// @brief get_mat4_at const
	/// Gets a const reference to the data at the specific index, cast as a mat4.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const glm::mat4& Datum::get_mat4_at(size_t index) const
	{
		if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in get_mat4_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_mat4_at const."); }
		if (isExternal) { return ((glm::mat4*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(glm::mat4*)nodePtr->nodeData;
	}

	/// @brief get_string_at const
	/// Gets a const reference to the data at the specific index, cast as a string.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const std::string& Datum::get_string_at(size_t index) const
	{
		if (mType != DatumType::String) { throw std::invalid_argument("Wrong Datum type in get_string_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_string_at const."); }
		if (isExternal) { return ((std::string*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(std::string*)nodePtr->nodeData;
	}

	/// @brief get_int_at
	/// Gets a reference to the data at the specific index, cast as an int.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	int& Datum::get_int_at(size_t index)
	{
		if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in get_int_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_int_at."); }
		if (isExternal) { return ((int*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(int*)nodePtr->nodeData;
	}

	/// @brief get_float_at
	/// Gets a const reference to the data at the specific index, cast as a float.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	float& Datum::get_float_at(size_t index)
	{
		if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in get_float_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_float_at."); }
		if (isExternal) { return ((float*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(float*)nodePtr->nodeData;
	}

	/// @brief get_vec4_at
	/// Gets a const reference to the data at the specific index, cast as a vec4.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	glm::vec4& Datum::get_vec4_at(size_t index)
	{
		if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in get_vec4_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_vec4_at."); }
		if (isExternal) { return ((glm::vec4*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(glm::vec4*)nodePtr->nodeData;
	}

	/// @brief get_mat4_at
	/// Gets a const reference to the data at the specific index, cast as a mat4.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	glm::mat4& Datum::get_mat4_at(size_t index)
	{
		if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in get_mat4_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_mat4_at."); }
		if (isExternal) { return ((glm::mat4*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(glm::mat4*)nodePtr->nodeData;
	}

	/// @brief get_string_at
	/// Gets a const reference to the data at the specific index, cast as a string.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	std::string& Datum::get_string_at(size_t index)
	{
		if (mType != DatumType::String) { throw std::invalid_argument("Wrong Datum type in get_string_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_string_at."); }
		if (isExternal) { return ((std::string*)headNode->nodeData)[index]; }
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(std::string*)nodePtr->nodeData;
	}

	/// @brief get_int_at_as_string
	/// Gets a string representation of the int at the specified index
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const string representation of the data at the index
	const std::string Datum::get_int_at_as_string(size_t index) const
	{
		if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in get_int_at_as_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_int_at_as_string."); }
		return IntToString(get_int_at(index));
	}

	/// @brief get_float_at_as_string
	/// Gets a string representation of the float at the specified index
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const string representation of the data at the index
	const std::string Datum::get_float_at_as_string(size_t index) const
	{
		if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in get_float_at_as_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_float_at_as_string."); }
		return FloatToString(get_float_at(index));
	}

	/// @brief get_vec4_at_as_string
	/// Gets a string representation of the vec4 at the specified index
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const string representation of the data at the index
	const std::string Datum::get_vec4_at_as_string(size_t index) const
	{
		if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in get_vec4_at_as_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_vec4_at_as_string."); }
		return Vec4ToString(get_vec4_at(index));
	}

	/// @brief get_mat4_at_as_string
	/// Gets a string representation of the mat4 at the specified index
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const string representation of the data at the index
	const std::string Datum::get_mat4_at_as_string(size_t index) const
	{
		if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in get_matma4_at_as_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_mat4_at_as_string."); }
		return Mat4ToString(get_mat4_at(index));
	}
#pragma endregion

#pragma region Setters
	/// @brief set_int_at
	/// Sets the element at the specified index as a copy of the passed in int
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param i: the data to set at that index
	/// @return Returns a reference to the element that was just set
	int& Datum::set_int_at(size_t index, int i)
	{
		if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in set_int_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_int_at."); }
		if (isExternal) 
		{ 
			((int*)headNode->nodeData)[index] = i;
			return ((int*)headNode->nodeData)[index]; 
		}
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(int*)nodePtr->nodeData = i;
		return *(int*)nodePtr->nodeData;
	}

	/// @brief set_float_at
	/// Sets the element at the specified index as a copy of the passed in float
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param f: the data to set at that index
	/// @return Returns a reference to the element that was just set
	float& Datum::set_float_at(size_t index, float f)
	{
		if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in set_float_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_float_at."); }
		if (isExternal) 
		{ 
			((float*)headNode->nodeData)[index] = f;
			return ((float*)headNode->nodeData)[index]; 
		}
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(float*)nodePtr->nodeData = f;
		return *(float*)nodePtr->nodeData;
	}

	/// @brief set_vec4_at
	/// Sets the element at the specified index as a copy of the passed in vec4
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param vec: the data to set at that index
	/// @return Returns a reference to the element that was just set
	glm::vec4& Datum::set_vec4_at(size_t index, const glm::vec4& vec)
	{
		if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in set_vec4_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_vec4_at."); }
		if (isExternal) 
		{ 
			((glm::vec4*)headNode->nodeData)[index] = vec;
			return ((glm::vec4*)headNode->nodeData)[index]; 
		}
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(glm::vec4*)nodePtr->nodeData = vec;
		return *(glm::vec4*)nodePtr->nodeData;
	}

	/// @brief set_mat4_at
	/// Sets the element at the specified index as a copy of the passed in mat4
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param mat: the data to set at that index
	/// @return Returns a reference to the element that was just set
	glm::mat4& Datum::set_mat4_at(size_t index, const glm::mat4& mat)
	{
		if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in set_mat4_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_mat4_at."); }
		if (isExternal) 
		{ 
			((glm::mat4*)headNode->nodeData)[index] = mat;
			return ((glm::mat4*)headNode->nodeData)[index]; 
		}
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(glm::mat4*)nodePtr->nodeData = mat;
		return *(glm::mat4*)nodePtr->nodeData;
	}

	/// @brief set_string_at
	/// Sets the element at the specified index as a copy of the passed in string
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param str: the data to set at that index
	/// @return Returns a reference to the element that was just set
	std::string& Datum::set_string_at(size_t index, const std::string& str)
	{
		if (mType != DatumType::String) { throw std::invalid_argument("Wrong Datum type in set_string_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_string_at."); }
		if (isExternal) 
		{ 
			((std::string*)headNode->nodeData)[index] = str;
			return ((std::string*)headNode->nodeData)[index]; 
		}
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(std::string*)nodePtr->nodeData = str;
		return *(std::string*)nodePtr->nodeData;
	}

	/// @brief set_int_at_using_string
	/// Sets the element at the specified index as an int copy of the string representation
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param str: the string representation of the data to be set
	/// @return Returns a reference to the element that was just set
	int& Datum::set_int_at_using_string(size_t index, const std::string& str)
	{
		if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in set_int_at_using_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_int_at_using_string."); }
		return set_int_at(index, StringToInt(str));
	}

	/// @brief set_float_at_using_string
	/// Sets the element at the specified index as an float copy of the string representation
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param str: the string representation of the data to be set
	/// @return Returns a reference to the element that was just set
	float& Datum::set_float_at_using_string(size_t index, const std::string& str)
	{
		if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in set_float_at_using_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_float_at_using_string."); }
		return set_float_at(index, StringToFloat(str));
	}

	/// @brief set_vec4_at_using_string
	/// Sets the element at the specified index as an vec4 copy of the string representation
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param str: the string representation of the data to be set
	/// @return Returns a reference to the element that was just set
	glm::vec4& Datum::set_vec4_at_using_string(size_t index, const std::string& str)
	{
		if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in set_vec4_at_using_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_vec4_at_using_string."); }
		return set_vec4_at(index, StringToVec4(str));
	}

	/// @brief set_mat4_at_using_string
	/// Sets the element at the specified index as an mat4 copy of the string representation
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param str: the string representation of the data to be set
	/// @return Returns a reference to the element that was just set
	glm::mat4& Datum::set_mat4_at_using_string(size_t index, const std::string& str)
	{
		if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in set_mat4_at_using_string."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_mat4_at_using_string."); }
		return set_mat4_at(index, StringToMat4(str));
	}
#pragma endregion

#pragma region ExternalStorage
	/// @brief set_external_int_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_int_storage(int* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_int_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::Integer) { throw std::invalid_argument("Tried to set_external_int_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::Integer;
		mSize = size;
		isExternal = true;
	}

	/// @brief set_external_float_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_float_storage(float* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_float_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::Float) { throw std::invalid_argument("Tried to set_external_float_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::Float;
		mSize = size;
		isExternal = true;
	}

	/// @brief set_external_vec4_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_vec4_storage(glm::vec4* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_vec4_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::Vector4) { throw std::invalid_argument("Tried to set_external_vec4_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::Vector4;
		mSize = size;
		isExternal = true;
	}

	/// @brief set_external_mat4_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_mat4_storage(glm::mat4* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_mat4_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::Matrix4) { throw std::invalid_argument("Tried to set_external_mat4_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::Matrix4;
		mSize = size;
		isExternal = true;
	}

	/// @brief set_external_string_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_string_storage(std::string* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_string_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::String) { throw std::invalid_argument("Tried to set_external_string_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::String;
		mSize = size;
		isExternal = true;
	}

	/// @brief set_external_void_storage
	/// Sets the storage to some externally-allocated memory
	/// Wrapper function to just call the correct set_external_storage
	/// Does not do any error checking itself
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	/// @param type: the DatumType of data being pointed to
	void Datum::set_external_void_storage(void* ptr, size_t size, DatumType type)
	{
		switch (type)
		{
		case DatumType::Integer:
			set_external_int_storage((int*)ptr, size);
			break;
		case DatumType::Float:
			set_external_float_storage((float*)ptr, size);
			break;
		case DatumType::Vector4:
			set_external_vec4_storage((glm::vec4*)ptr, size);
			break;
		case DatumType::Matrix4:
			set_external_mat4_storage((glm::mat4*)ptr, size);
			break;
		case DatumType::String:
			set_external_string_storage((std::string*)ptr, size);
			break;
		case DatumType::Table:
			push_scope_back((ScopePtr)ptr);
			break;
		case DatumType::Pointer:
			push_RTTI_back((RTTIPtr)ptr);
			break;
		case DatumType::Unknown:
			throw std::invalid_argument("Passed in Unknown to Datum::set_external_void_storage.");
			break;
		default:
			throw std::invalid_argument("Invalid argument passed into Datum::set_external_void_storage.");
			break;
		}

	}
#pragma endregion

#pragma region SizeAndType
	/// @brief is_empty
	/// Predicate for if the Datum is empty. That is, the size is 0. 
	/// Does not care about what DatumType the Datum is, or even if it's Unknown
	/// @return Returns true if Datum is empty
	bool Datum::is_empty() const
	{
		return mSize == 0;
	}

	/// @brief size
	/// @return Returns the number of elemnts in the Datum
	size_t Datum::size() const
	{
		return mSize;
	}

	/// @brief set_size
	/// Sets the size of the Datum.
	/// If requested size is smaller than current size, will free all excess memory
	/// If requested size is equal to current size, no op
	/// If request size is larger than current size, will create default-constructed items up to the requested size
	/// @param newSize: The new size of the Datum
	void Datum::set_size(size_t newSize)
	{
		// If equal, just return
		if (newSize == mSize) { return; }
		// If zero, just clear
		if (newSize == 0) { clear(); }
		// If less than current size, delete
		if (newSize < mSize)
		{
			for (size_t i = mSize; i > newSize; --i) { pop_back(); }
		}
		// If greater than current size, default-construct
		else
		{
			// For each size difference, push back a new node that's default construction
			for (size_t i = mSize; i < newSize; ++i)
			{
				switch (mType)
				{
				case DatumType::Integer:
					push_int_back(int());
					break;
				case DatumType::Float:
					push_float_back(float());
					break;
				case DatumType::Vector4:
					push_vec4_back(glm::vec4());
					break;
				case DatumType::Matrix4:
					push_mat4_back(glm::mat4());
					break;
				case DatumType::String:
					push_string_back(std::string());
					break;
				case DatumType::Unknown:
					throw std::invalid_argument("Passed in Unknown to set_size.");
					break;
				default:
					throw std::invalid_argument("Invalid argument passed into set_size.");
					break;
				}
			}
			return;
		}
	}

	/// @brief set_type
	/// Sets the type of the Datum if it is Unknown.
	/// If the Datum is not Unknown and does not match, throws an exception
	/// @param type: The DatumType to set the Datum to
	void Datum::set_type(DatumType type)
	{
		// If it's the same type, no op. Just return
		if (mType == type) { return; }
		// If it's not unknown, then we throw an exception
		if (mType != DatumType::Unknown) { throw std::invalid_argument("Invalid call to set_type"); }
		mType = type;
	}

	/// @brief get_type
	/// @return Returns the DatumType of the Datum
	DatumType Datum::get_type() const
	{
		return mType;
	}

	/// @brief is_unknown
	/// Predicate for if the Datum still has an Unknown type.
	/// That is, its data has not been set yet.
	/// @return Returns true if the Datum's type is Unknown
	bool Datum::is_unknown() const
	{
		return mType == DatumType::Unknown;
	}

	/// @brief is_compatible_with
	/// Predicate for if the Datum is compatible with a certain DatumType.
	/// For now, the Datum is only compatible with DatumTypes that exactly matches its own DatumType
	/// @param type: DatumType to check if the Datum is compatible with
	/// @return Returns true if the Datum is compatible with the DatumType
	bool Datum::is_compatible_with(DatumType type) const
	{
		return type == mType;
	}

	/// @brief is_external
	/// Predicate for if the Datum is using external memory
	/// @return Returns true if the Datum is using exeternal memory
	bool Datum::is_external() const
	{
		return isExternal;
	}
#pragma endregion

#pragma region Push
	/// @brief push_int_front
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param i: the int to copy into Datum
	void Datum::push_int_front(int i)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_int_front(int) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Integer; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in push_int_front(int)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) int(i);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_float_back
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param f: the float to copy into Datum
	void Datum::push_float_front(float f)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_float_front(float) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Float; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in push_float_front(float)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) float(f);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_vec4_back
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param vec: the vec4 to copy into Datum
	void Datum::push_vec4_front(const glm::vec4& vec)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_vec4_front(vec4) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Vector4; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in push_vec4_front(vec4)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) glm::vec4(vec);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_mat4_back
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param mat: the mat4 to copy into Datum
	void Datum::push_mat4_front(const glm::mat4& mat)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_mat4_front(mat4) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Matrix4; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in push_mat4_front(mat4)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) glm::mat4(mat);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_string_back
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: the string to copy into Datum
	void Datum::push_string_front(const std::string& str)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_string_front(string) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::String; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::String) { throw std::invalid_argument("Wrong Datum type in push_string_front(string)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) std::string(str);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_int_back
	/// Makes a new element at the end of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param i: the int to copy into Datum
	void Datum::push_int_back(int i)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_int_back(int) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Integer; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in push_int_back(int)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) int(i);
		int* testPtr = (int*)lastNode->nodeData;
		int testInt = *testPtr;
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_float_back
	/// Makes a new element at the end of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param f: the float to copy into Datum
	void Datum::push_float_back(float f)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_float_back(float) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Float; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in push_float_back(float)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) float(f);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief push_vec4_back
	/// Makes a new element at the end of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param vec: the vec4 to copy into Datum
	void Datum::push_vec4_back(const glm::vec4& vec)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_vec4_back(vec4) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Vector4; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in push_vec4_back(vec4)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) glm::vec4(vec);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief push_mat4_back
	/// Makes a new element at the end of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param mat: the mat4 to copy into Datum
	void Datum::push_mat4_back(const glm::mat4& mat)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_mat4_back(mat4) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Matrix4; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in push_mat4_back(mat4)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) glm::mat4(mat);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief push_string_back
	/// Makes a new element at the end of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: the string to copy into Datum
	void Datum::push_string_back(const std::string& str)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_string_back(string) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::String; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::String) { throw std::invalid_argument("Wrong Datum type in push_string_back(string)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) std::string(str);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief push_int_front(string)
	/// Makes a new element at the start of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the int to copy into Datum
	void Datum::push_int_front(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_int_front(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in push_int_front(string)."); }
		push_int_front(StringToInt(str));
	}

	/// @brief push_float_back(string)
	/// Makes a new element at the start of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the float to copy into Datum
	void Datum::push_float_front(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_float_front(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in push_float_front(string)."); }
		push_float_front(StringToFloat(str));
	}

	/// @brief push_vec4_back(string)
	/// Makes a new element at the start of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the vec to copy into Datum
	void Datum::push_vec4_front(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_vec4_front(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in push_vec4_front(string)."); }
		push_vec4_front(StringToVec4(str));
	}

	/// @brief push_mat4_back(string)
	/// Makes a new element at the start of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the mat to copy into Datum
	void Datum::push_mat4_front(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_mat4_front(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in push_mat4_front(string)."); }
		push_mat4_front(StringToMat4(str));
	}

	/// @brief push_int_back(string)
	/// Makes a new element at the end of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the int to copy into Datum
	void Datum::push_int_back(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_int_back(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Integer) { throw std::invalid_argument("Wrong Datum type in push_int_back(string)."); }
		push_int_back(StringToInt(str));
	}

	/// @brief push_float_back(string)
	/// Makes a new element at the end of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the float to copy into Datum
	void Datum::push_float_back(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_float_back(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Float) { throw std::invalid_argument("Wrong Datum type in push_float_back(string)."); }
		push_float_back(StringToFloat(str));
	}

	/// @brief push_vec4_back(string)
	/// Makes a new element at the end of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the vec to copy into Datum
	void Datum::push_vec4_back(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_vec4_back(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Vector4) { throw std::invalid_argument("Wrong Datum type in push_vec4_back(string)."); }
		push_vec4_back(StringToVec4(str));
	}

	/// @brief push_mat4_back(string)
	/// Makes a new element at the end of the Datum that holds data represented by the string
	/// Will set the DatumType if currently Unkonwn
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param str: a string representation the mat to copy into Datum
	void Datum::push_mat4_back(const std::string& str)
	{
		if (isExternal == true) { throw std::invalid_argument("push_mat4_back(string) not allowed on external memory."); }
		if (mType != DatumType::Unknown && mType != DatumType::Matrix4) { throw std::invalid_argument("Wrong Datum type in push_mat4_back(string)."); }
		push_mat4_back(StringToMat4(str));
	}
#pragma endregion

#pragma region RemoveElements
	/// @brief clear
	/// Removes all elements in the Datum.
	/// Does not reset the type back to Unknown
	/// External Memory: Just frees the single node
	void Datum::clear()
	{
		// If size is zero, no-op
		if (mSize == 0) { return; }
		// If external, just free the one node
		if (isExternal == true) { FreeExternalNode(headNode); }
		// If internal, then go through and free every node
		else
		{
			// pointer to hold data to delete
			DatumNode* deleteNode = nullptr;
			// Keep going until headNode reaches nullptr
			while (headNode != nullptr)
			{
				// move headNode to the next node
				deleteNode = headNode;
				headNode = headNode->next;
				// Destruct data. Free data. Free node.
				FreeNode(deleteNode);
			}
		}
		// Reset all variables except DatumType
		mSize = 0;
		headNode = nullptr;
		lastNode = nullptr;
		//mType = DatumType::Unknown;
	}

	/// @brief pop_front
	/// Removes the first element of the Datum
	/// If the size is zero, throws an exception
	/// External Memory: Disallowed
	/// Note: Removing the last element does not reset the DatumType back to Unknown.
	void Datum::pop_front()
	{
		if (isExternal == true) { throw std::invalid_argument("pop_front not allowed on external memory."); }
		// Throw exception if Datum is empty
		if (mSize == 0) { throw std::out_of_range("Called pop_back on an empty Datum"); }
		// Otherwise, increment headNode
		DatumNode* deleteNode = headNode;
		headNode = headNode->next;
		// Remove first element
		FreeNode(deleteNode);
		// Reduce size
		--mSize;
		// If Datum is now empty, make sure lastNode is also null
		if (mSize == 0) { lastNode = nullptr; }
	}

	/// @brief pop_back
	/// Removes the last element of the Datum
	/// If the size is zero, throws an exception
	/// Note: Removing the last element does not reset the DatumType back to Unknown.
	/// External Memory: Disallowed
	/// TODO: This is currently an O(n) operation. Consider changing to doublely-linked list
	void Datum::pop_back()
	{
		if (isExternal == true) { throw std::invalid_argument("pop_back not allowed on external memory."); }
		// Throw exception if Datum is empty
		if (mSize == 0) { throw std::out_of_range("Called pop_back on an empty Datum"); }
		// Otherwise, remove last element
		FreeNode(lastNode);
		// Reduce size
		--mSize;
		// If Datum is now empty, null headNode and lastNode
		if (mSize == 0) { headNode = nullptr; lastNode = nullptr; }
		// Otherwise, re-set lastNode, and mark it as the last node
		else 
		{ 
			lastNode = TraverseLinkedListBy(headNode, mSize - 1); 
			lastNode->next = nullptr;
		}
	}
#pragma endregion

#pragma region EqualityOperator
	/// @brief Equality operator operator==
	/// Compares this Datum with another Datum. Checks that all elements are equal.
	/// Will return false if the DatumTypes do not match
	/// @param rhs: the Datum to compare with
	/// @return Returns true if the Datums are equal
	bool Datum::operator==(const Datum& rhs) const
	{
		// if size or type are different, then return false immediately
		if (rhs.mSize != this->mSize || rhs.mType != this->mType) { return false; }
		// Go through each element and check if they are equal
		DatumNode* myNode = this->headNode;
		DatumNode* rhsNode = rhs.headNode;
		for (size_t i = 0; i < mSize; ++i)
		{
			if (!NodesAreEqual(myNode, rhsNode)) { return false; }
		}
		// Gone through all nodes and they all match. Return true
		return true;
	}

	/// @brief Inequality operator operator!=
	/// Compares this Datum with another Datum. Checks that at least one element is not equal.
	/// Will return true if the DatumTypes do not match
	/// @param rhs: the Datum to compare with
	/// @return Returns true if the Datums are not equal
	bool Datum::operator!=(const Datum& rhs) const
	{
		// returns the opposite of operator ==
		return !this->operator==(rhs);
	}

	/// @brief Scalar Equality operator==(int)
	/// Compares this Datum with a scalar int. Checks that Datum has size of 1 and the single element is equal
	/// @param i: scalar to compare with
	/// @return Returns true if the Datum has only one element, and it is equal to the scalar
	bool Datum::operator==(int i) const
	{
		// Checks size and type
		if (mSize != 1 || mType != DatumType::Integer) { return false; }
		// check the single element
		return i == *(int*)lastNode->nodeData;
	}

	/// @brief Scalar Inequality operator!=(int)
	/// Compares this Datum with a scalar int. Checks that the Datum does not only hold one element equal to it
	/// @param i: scalar to compare with
	/// @return Returns true if the Datum does not hold a single element that matches the scalar
	bool Datum::operator!=(int i) const
	{
		return !this->operator==(i);
	}

	/// @brief Scalar Equality operator==(float)
	/// Compares this Datum with a scalar float. Checks that Datum has size of 1 and the single element is equal
	/// @param f: scalar to compare with
	/// @return Returns true if the Datum has only one element, and it is equal to the scalar
	bool Datum::operator==(float f) const
	{
		// Checks size and type
		if (mSize != 1 || mType != DatumType::Float) { return false; }
		// check the single element
		return f == *(float*)lastNode->nodeData;
	}

	/// @brief Scalar Inequality operator!=(float)
	/// Compares this Datum with a scalar float. Checks that the Datum does not only hold one element equal to it
	/// @param f: scalar to compare with
	/// @return Returns true if the Datum does not hold a single element that matches the scalar
	bool Datum::operator!=(float f) const
	{
		return !this->operator==(f);
	}

	/// @brief Scalar Equality operator==(vec4)
	/// Compares this Datum with a scalar vec4. Checks that Datum has size of 1 and the single element is equal
	/// @param vec: scalar to compare with
	/// @return Returns true if the Datum has only one element, and it is equal to the scalar
	bool Datum::operator==(const glm::vec4& vec) const
	{
		// Checks size and type
		if (mSize != 1 || mType != DatumType::Vector4) { return false; }
		// check the single element
		return vec == *(glm::vec4*)lastNode->nodeData;
	}

	/// @brief Scalar Inequality operator!=(vec4)
	/// Compares this Datum with a scalar vec4. Checks that the Datum does not only hold one element equal to it
	/// @param vec: scalar to compare with
	/// @return Returns true if the Datum does not hold a single element that matches the scalar
	bool Datum::operator!=(const glm::vec4& vec) const
	{
		return !this->operator==(vec);
	}

	/// @brief Scalar Equality operator==(mat4)
	/// Compares this Datum with a scalar mat4. Checks that Datum has size of 1 and the single element is equal
	/// @param mat: scalar to compare with
	/// @return Returns true if the Datum has only one element, and it is equal to the scalar
	bool Datum::operator==(const glm::mat4& mat) const
	{
		// Checks size and type
		if (mSize != 1 || mType != DatumType::Matrix4) { return false; }
		// check the single element
		return mat == *(glm::mat4*)lastNode->nodeData;
	}

	/// @brief Scalar Inequality operator!=(mat4)
	/// Compares this Datum with a scalar mat4. Checks that the Datum does not only hold one element equal to it
	/// @param mat: scalar to compare with
	/// @return Returns true if the Datum does not hold a single element that matches the scalar
	bool Datum::operator!=(const glm::mat4& mat) const
	{
		return !this->operator==(mat);
	}

	/// @brief Scalar Equality operator==(string)
	/// Compares this Datum with a scalar string. Checks that Datum has size of 1 and the single element is equal
	/// @param str: scalar to compare with
	/// @return Returns true if the Datum has only one element, and it is equal to the scalar
	bool Datum::operator==(const std::string& str) const
	{
		// Checks size and type
		if (mSize != 1 || mType != DatumType::String) { return false; }
		// check the single element
		return str == *(std::string *)lastNode->nodeData;
	}

	/// @brief Scalar Inequality operator!=(string)
	/// Compares this Datum with a scalar string. Checks that the Datum does not only hold one element equal to it
	/// @param str: scalar to compare with
	/// @return Returns true if the Datum does not hold a single element that matches the scalar
	bool Datum::operator!=(const std::string& str) const
	{
		return !this->operator==(str);
	}
#pragma endregion

#pragma region Scope
	/// @brief Converting constructor: scope
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param scope: scope* to copy into the Datum
	Datum::Datum(const ConstScopePtr& scope) : mType(DatumType::Table), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) ConstScopePtr(scope);
	}

	/// @brief set_scope_at
	/// Sets the element at the specified index as a copy of the passed in scope*
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param scope: the data to set at that index
	/// @return Returns a reference to the element that was just set
	typename ScopePtr& Datum::set_scope_at(size_t index, const ConstScopePtr& scope)
	{
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in set_scope_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_scope_at."); }
		/* Scope cannot be external
		if (isExternal) { ... } */
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(ConstScopePtr*)nodePtr->nodeData = scope;
		return *(ScopePtr*)nodePtr->nodeData; // Note, casting to a mutable ScopePtr
	}

	/// @brief Scalar assignment operator=(scope)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param scope: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(const ConstScopePtr& scope)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(scope) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Table)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(scope) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Table;
		// Create a single node with a copy of the data
		push_scope_back(scope);
		// Return this
		return *this;
	}

	/// @brief get_scope_at const
	/// Gets a const reference to the data at the specific index, cast as an scope*.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	typename const ConstScopePtr& Datum::get_scope_at(size_t index) const
	{
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in get_scope_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_scope_at const."); }
		/* if (isExternal) { return ((std::string*)headNode->nodeData)[index]; } */ // Scope cannot be external
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(ConstScopePtr*)nodePtr->nodeData;
	}

	/// @brief get_scope_at
	/// Gets a reference to the data at the specific index, cast as an scope*.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	typename ScopePtr& Datum::get_scope_at(size_t index)
	{
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in get_scope_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_scope_at."); }
		/* if (isExternal) { return ((std::string*)headNode->nodeData)[index]; } */ // TODO: can scope be external
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(ScopePtr*)nodePtr->nodeData; // Note: casting to a mutable ScopePtr
	}

	/// @brief operator[]
	/// Gets a reference to the actual Scope at the specified index, not just the scope*
	/// Only works on Scope Datums. Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a refernece to the scope itself
	Scope& Datum::operator[](size_t index)
	{
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in operator[]."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in operator[]."); }
		return *get_scope_at(index);
	}

	/// @brief operator[] const
	/// Gets a const reference to the actual Scope at the specified index, not just the scope*
	/// Only works on Scope Datums. Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const refernece to the scope itself
	const Scope& Datum::operator[](size_t index) const
	{
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in operator[] const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in operator[] const."); }
		return *get_scope_at(index);
	}

	/// @brief push_scope_front
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param scope: the scope* to copy into Datum
	void Datum::push_scope_front(const ConstScopePtr& scope)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_scope_front(scope) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Table; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in push_scope_front(scope)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) ConstScopePtr(scope);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_scope_back
	/// Makes a new element at the back of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param scope: the scope* to copy into Datum
	void Datum::push_scope_back(const ConstScopePtr& scope)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_scope_back(scope) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Table; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in push_scope_back(scope)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) ConstScopePtr(scope);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief find_scope
	/// Goes through this datum and looks for a scope pointer that matches the input
	/// Will throw an exception if the DatumType is not corect
	/// @param scopePtrToFind: The scope pointer to look for
	/// @param outIndex: Output int with the index that the scope is at
	/// @return: True if found. False if not found
	bool Datum::find_scope(const ConstScopePtr& scopePtrToFind, size_t* outIndex) const
	{
		// error checking: wrong DatumType
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in find_scope."); }
		// Set initial settings to traverse linked list
		DatumNode* currentNode = headNode;
		*outIndex = 0;
		// Traverse entire linked list and look for a ScopePtr that matches
		while (currentNode != nullptr)
		{
			// If found, then return true and outIndex should hold the index of the scope
			if (scopePtrToFind == *(ConstScopePtr*)currentNode->nodeData) { return true; }
			// Else, keep going
			++(*outIndex);
			currentNode = currentNode->next;
		}
		return false;
	}

	/// @brief remove_scope_at
	/// Will remove the scope at the specific index of the Datum.
	/// Will throw an exception if the DatumType is not correct.
	/// Will throw an exception if the index is too large.
	/// External Memory: Disallowed
	/// @param index: the index at which to delete the scope
	void Datum::remove_scope_at(size_t index)
	{
		/// Error checking: isExternal, DatumType, index out of bounds
		if (isExternal == true) { throw std::invalid_argument("remove_scope_at(index) not allowed on external memory."); }
		if (mType != DatumType::Table) { throw std::invalid_argument("Wrong Datum type in remove_scope_at(index)."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in remove_scope_at."); }
		// Edge case: at index zero
		if (index == 0) { pop_front(); }
		else
		{
			// Grab the node right before the node to remove
			DatumNode* previousNode = TraverseLinkedListBy(headNode, index - 1);
			// Mark the next node (the node at index) for deletion
			DatumNode* deleteNode = previousNode->next;
			// Set the next of previousNode to skip over deleteNode
			previousNode->next = deleteNode->next;
			// Delete deletenode
			FreeNode(deleteNode);
			// Reduce size
			--mSize;
		}
	}
#pragma endregion

#pragma region RTTI
	/// @brief Converting constructor: rtti
	/// Creates a datum with a size of 1 of the corresponding type. The single element is a copy of the parameter that is passed in
	/// @param rtti: RTTI* to copy into the Datum
	Datum::Datum(const RTTIPtr& rtti) : mType(DatumType::Pointer), headNode(nullptr), lastNode(nullptr), mSize(1)
	{
		// Create a new node. Set headNode and lastNode to the new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		headNode = lastNode;
		// placement new data in the new node
		new(lastNode->nodeData) RTTIPtr(rtti);
	}

	/// @brief set_RTTI_at
	/// Sets the element at the specified index as a copy of the passed in RTTI*
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to set
	/// @param rtti: the data to set at that index
	/// @return Returns a reference to the element that was just set
	RTTIPtr& Datum::set_RTTI_at(size_t index, const RTTIPtr& rtti)
	{
		if (mType != DatumType::Pointer) { throw std::invalid_argument("Wrong Datum type in set_RTTI_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in set_RTTI_at."); }
		/* RTTI cannot be external
		if (isExternal) {...} */
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		*(RTTIPtr*)nodePtr->nodeData = rtti;
		return *(RTTIPtr*)nodePtr->nodeData;
	}

	/// @brief Scalar assignment operator=(rtti)
	/// Sets the Datum to be a one-element Datum with a single element, which is a copy of the parameter
	/// Note: If the DatumType is incorrect, throws an error
	/// External Memory: Disallowed
	/// @param rtti: the data to be copied into the Datum
	/// @return Returns a reference to *this
	Datum& Datum::operator=(const RTTIPtr& rtti)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("operator=(RTTI) not allowed on external memory."); }
		// Check if the types are mismatched (but it's ok if this datum is still Unknown)
		if (mType != DatumType::Unknown && mType != DatumType::Pointer)
		{
			throw std::invalid_argument("Type of Datum passed into operator=(RTTI) does not match.");
		}
		// Delete old data
		clear();
		// Set the type (in case it was Unknown)
		mType = DatumType::Pointer;
		// Create a single node with a copy of the data
		push_RTTI_back(rtti);
		// Return this
		return *this;
	}

	/// @brief get_RTTI_at const
	/// Gets a const reference to the data at the specific index, cast as an RTTI*.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a const reference to the data
	const RTTIPtr& Datum::get_RTTI_at(size_t index) const
	{
		if (mType != DatumType::Pointer) { throw std::invalid_argument("Wrong Datum type in get_RTTI_at const."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_RTTI_at const."); }
		/* if (isExternal) { return ((std::string*)headNode->nodeData)[index]; } */ // RTTI cannot be external
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(RTTIPtr*)nodePtr->nodeData;
	}

	/// @brief get_RTTI_at
	/// Gets a reference to the data at the specific index, cast as an RTTI*.
	/// Will throw an exception if DatumType does not match.
	/// Will throw an exception if index is greater than or equal to number of elements.
	/// @param index: the index to get the data from
	/// @return Returns a reference to the data
	RTTIPtr& Datum::get_RTTI_at(size_t index)
	{
		if (mType != DatumType::Pointer) { throw std::invalid_argument("Wrong Datum type in get_RTTI_at."); }
		if (index >= mSize) { throw std::out_of_range("Index is too large in get_RTTI_at."); }
		/* if (isExternal) { return ((std::string*)headNode->nodeData)[index]; } */ // TODO: can scope be external
		DatumNode* nodePtr = TraverseLinkedListBy(headNode, index);
		return *(RTTIPtr*)nodePtr->nodeData;
	}

	/// @brief push_RTTI_front
	/// Makes a new element at the front of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param rtti: the RTTI* to copy into Datum
	void Datum::push_RTTI_front(const RTTIPtr& rtti)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_RTTI_front(rtti) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Pointer; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Pointer) { throw std::invalid_argument("Wrong Datum type in push_RTTI_front(rtti)."); }
		// Make a new node
		headNode = CreateNodeBefore(headNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(headNode->nodeData) RTTIPtr(rtti);
		// If this was the first element, then set lastNode as well.
		if (mSize == 0) { lastNode = headNode; }
		// Increment size.
		++mSize;
	}

	/// @brief push_RTTI_back
	/// Makes a new element at the back of the Datum that holds a copy of the parameter.
	/// Will set the DatumType if currenlty Unknown
	/// Will throw an exception if the DatumType is not correct.
	/// External Memory: Disallowed
	/// @param rtti: the RTTI* to copy into Datum
	void Datum::push_RTTI_back(const RTTIPtr& rtti)
	{
		/// External Memory: Disallowed
		if (isExternal == true) { throw std::invalid_argument("push_RTTI_back(rtti) not allowed on external memory."); }
		// If DatumType is currently Unknown, then set it
		if (mType == DatumType::Unknown) { mType = DatumType::Pointer; }
		// If DatumType is anything else, then throw an error
		else if (mType != DatumType::Pointer) { throw std::invalid_argument("Wrong Datum type in push_RTTI_back(rtti)."); }
		// Make a new node
		lastNode = CreateNodeAfter(lastNode, GetSizeOfDatumType(mType));
		// Set data in that new node
		new(lastNode->nodeData) RTTIPtr(rtti);
		// If this was the first element, then set headNode as well.
		if (mSize == 0) { headNode = lastNode; }
		// Increment size
		++mSize;
	}

	/// @brief set_external_RTTI_storage
	/// Sets the storage to some externally-allocated memory
	/// Will throw an exception if the Datum is already internally-allocated (i.e. not Unknown and !isExternal)
	/// Will throw an exception if the DatumType does not match
	/// @param ptr: the start of the external array
	/// @param size: the size of the external array
	void Datum::set_external_RTTI_storage(RTTIPtr* ptr, size_t size)
	{
		// If not Unknown, we have to go through some extra checks
		if (mType != DatumType::Unknown)
		{
			// Throw exception if Datum is already internally-allocated
			if (isExternal == false) { throw std::invalid_argument("Tried to set_external_RTTI_storage on an internal Datum."); }
			// Throw exception if type-mistmatch
			if (mType != DatumType::Pointer) { throw std::invalid_argument("Tried to set_external_RTTI_storage on an incorrectly typed Datum."); }
		}
		// Remove existing data in Datum
		clear();
		// Create a single DatumNode to hold the external memory
		headNode = CreateExternalNode();
		lastNode = headNode;
		lastNode->nodeData = ptr;
		// Set variables
		mType = DatumType::Pointer;
		mSize = size;
		isExternal = true;
	}
#pragma endregion

	// ----------------------------------------------
	// --------- DatumNode Helper functions----------
	// ----------------------------------------------
#pragma region DatumNodeHelperFunctions
	/// @brief CreateNode
	/// Helper function for allocating memory for new DatumNodes
	/// @param sizeToAllocate: The amount of memory to allocate within the DatumNode
	/// @return Returns a pointer to the new node
	Datum::DatumNode* Datum::CreateNode(size_t sizeToAllocate)
	{
		// Allocates new memory to hold a new node
		DatumNode* newNode = (DatumNode*)malloc(sizeof(DatumNode));
		// error checking: bad malloc.
		// TODO: does there need to be an exception or error message?
		if (newNode == nullptr) { return nullptr; }
		// Allocate new memory to hold the data within this node
		newNode->nodeData = malloc(sizeToAllocate);
		// set the next pointer of the node to nullptr
		newNode->next = nullptr;
		// Return the point to the new node
		return newNode;
	}
	/// @brief CreateNodeBefore
	/// Helper function for allocating memory for new DatumNodes
	/// Additionally, automatically puts this new node right before the reference node
	/// @param refNode: The node to create the new node before. Ignored if nullptr
	/// @param sizeToAllocate: The amount of memory to allocate within the DatumNode
	/// @return Returns a pointer to the new node
	Datum::DatumNode* Datum::CreateNodeBefore(DatumNode* refNode, size_t sizeToAllocate)
	{
		// Create a new node
		DatumNode* newNode = CreateNode(sizeToAllocate);
		// Set the new node's pointer to refNode. If refNode is nullptr, then just skip
		if (refNode != nullptr) { newNode->next = refNode; }
		return newNode;
	}
	/// @brief CreateNodeAfter
	/// Helper function for allocating memory for new DatumNodes
	/// Additionally, automatically puts this new node right after the reference node
	/// @param refNode: The node to create the new node after. Ignored if nullptr
	/// @param sizeToAllocate: The amount of memory to allocate within the DatumNode
	/// @return Returns a pointer to the new node
	Datum::DatumNode* Datum::CreateNodeAfter(DatumNode* refNode, size_t sizeToAllocate)
	{
		// Create a new node
		DatumNode* newNode = CreateNode(sizeToAllocate);
		// Set the refNode's pointer to this new node. If refNode is nullptr, then just skip
		if (refNode != nullptr) { refNode->next = newNode; }
		// return a pointer to this new node
		return newNode;
	}
	/// @brief CreateExternalNode
	/// Helper function for allocating memory for the headNode of an external Datum
	/// @return Returns a pointer to the new node
	Datum::DatumNode* Datum::CreateExternalNode()
	{
		// Allocates new memory to hold a new node
		DatumNode* newNode = (DatumNode*)malloc(sizeof(DatumNode));
		// error checking: bad malloc.
		// TODO: does there need to be an exception or error message?
		if (newNode == nullptr) { return nullptr; }
		// nodeData will contain a pointer to an external array. No malloc required
		newNode->nodeData = nullptr;
		// set the next pointer of the node to nullptr
		newNode->next = nullptr;
		// Return the point to the new node
		return newNode;
	}

	/// @brief GetSizeOfDatumType
	/// Returns the size required to hold one element of the corresponding DatumType
	/// @param datumType: The DatumType to get the size of
	/// @return Returns the size (in bytes) of the type represented by the DatumType
	size_t Datum::GetSizeOfDatumType(DatumType datumType) const
	{
		switch (datumType)
		{
		case DatumType::Integer:
			return sizeof(int);
		case DatumType::Float:
			return sizeof(float);
		case DatumType::Vector4:
			return sizeof(glm::vec4);
		case DatumType::Matrix4:
			return sizeof(glm::mat4);
		case DatumType::String:
			return sizeof(std::string);
		case DatumType::Table:
			return sizeof(ConstScopePtr);
		case DatumType::Pointer:
			return sizeof(RTTIPtr);
		case DatumType::Unknown:
			throw std::invalid_argument("Passed in Unknown to GetSizeOfDatumType.");
			break;
		default:
			throw std::invalid_argument("Invalid argument passed into GetSizeOfDatumType.");
			break;
		}
	}

	/// @brief TraverseLinkedListBy
	/// Traverses a linked list, and returns the node after a certain amount of nodes
	/// Note: will not check for nullptr. Assumes that size is valid
	/// @param startNode: the node to start at
	/// @param index: the number of nodes to travese. 0 will just return startNode
	/// @return Returns pointer to the destination node
	Datum::DatumNode* Datum::TraverseLinkedListBy(DatumNode* startNode, size_t index) const
	{
		DatumNode* result = startNode;
		for (size_t i = 0; i < index; ++i)
		{
			result = result->next;
		}
		return result;
	}

	/// @brief CopyDataFromToNode
	/// Copies the data held by one node to another node of a matching type.
	/// Note: type matching not enforced
	/// @param fromNode: node to copy data from
	/// @param toNode: node to copy data to
	void Datum::CopyDataFromToNode(DatumNode* fromNode, DatumNode* toNode)
	{
		switch (mType)
		{
		case DatumType::Integer:
			new(lastNode->nodeData) int(*(int*)(fromNode->nodeData));
			break;
		case DatumType::Float:
			new(lastNode->nodeData) float(*(float*)(fromNode->nodeData));
			break;
		case DatumType::Vector4:
			new(lastNode->nodeData) glm::vec4(*(glm::vec4*)(fromNode->nodeData));
			break;
		case DatumType::Matrix4:
			new(lastNode->nodeData) glm::mat4(*(glm::mat4*)(fromNode->nodeData));
			break;
		case DatumType::String:
			new(lastNode->nodeData) std::string(*(std::string*)(fromNode->nodeData));
			break;
		case DatumType::Table:
			// Shallow copy
			new(lastNode->nodeData) ConstScopePtr(*(ConstScopePtr*)(fromNode->nodeData));
			//throw std::invalid_argument("Tried to call copyDataFromToNode on a DatumType::Table. Datum does not know about Scope, so this job is left up to Scope.");
			break;
		case DatumType::Pointer:
			// Shallow copy
			new(lastNode->nodeData) RTTIPtr(*(RTTIPtr*)(fromNode->nodeData));
			break;
		case DatumType::Unknown:
			throw std::invalid_argument("Passed in Unknown to CopyDataFromToNode.");
			break;
		default:
			throw std::invalid_argument("Invalid argument passed into CopyDataFromToNode.");
			break;
		}
	}

	/// @brief NodesAreEqual
	/// Predicate for if the data stored within two nodes are equal.
	/// Assumes nodes are the same type and match this Datum's type.
	/// @param firstNode: first node to check equality
	/// @param secondNode: node to check quality against
	/// @return Returns true if the data stored inside the nodes are equal
	bool Datum::NodesAreEqual(DatumNode* firstNode, DatumNode* secondNode) const
	{
		void* firstPtr = firstNode->nodeData;
		void* secondPtr = secondNode->nodeData;
		switch (mType)
		{
		case DatumType::Integer:
			return *(int*)(firstPtr) == *(int*)(secondPtr);
		case DatumType::Float:
			return *(float*)(firstPtr) == *(float*)(secondPtr);
		case DatumType::Vector4:
			return *(glm::vec4*)(firstPtr) == *(glm::vec4*)(secondPtr);
		case DatumType::Matrix4:
			return *(glm::mat4*)(firstPtr) == *(glm::mat4*)(secondPtr);
		case DatumType::String:
			return *(std::string*)(firstPtr) == *(std::string*)(secondPtr);
			break;
		case DatumType::Table:
			// Only shallow comparison
			return *(ConstScopePtr*)(firstPtr) == *(ConstScopePtr*)(secondPtr);
			//throw std::invalid_argument("Tried to call NodesAreEqual on a DatumType::Table. Datum does not know about Scope::operator==, so this job is left up to Scope.");
			break;
		case DatumType::Pointer:
			// Only shallow comparison
			return *(RTTIPtr*)(firstPtr) == *(RTTIPtr*)(secondPtr);
			break;
		case DatumType::Unknown:
			throw std::invalid_argument("Passed in Unknown to NodesAreEqual.");
			break;
		default:
			throw std::invalid_argument("Invalid argument passed into NodesAreEqual.");
			break;
		}
	}

	/// @brief FreeNode
	/// Destructs the memory held by a node, then frees the pointer to that memory.
	/// Then, frees the node itself
	/// @param deleteNode: the node to free
	void Datum::FreeNode(DatumNode* deleteNode)
	{
		// Sanity check: if I accidentally called this on external memory
		if (isExternal) { throw std::invalid_argument("You fool you absolute buffoon you called FreeNode on an external Datum"); }
		// Destruct data held by old node
		switch (mType)
		{
		case DatumType::Integer:
			// destructor not necessary for primitive
			break;
		case DatumType::Float:
			// destructor not necessary for primitive
			break;
		case DatumType::Vector4:
			((glm::vec4*)(deleteNode->nodeData))->glm::vec4::~vec4();
			break;
		case DatumType::Matrix4:
			((glm::mat4*)(deleteNode->nodeData))->glm::mat4::~mat4();
			break;
		case DatumType::String:
			((std::string*)(deleteNode->nodeData))->std::string::~string();
			break;
		case DatumType::Table:
			((ConstScopePtr*)(deleteNode->nodeData))->~ConstScopePtr();
			break;
		case DatumType::Pointer:
			((RTTIPtr*)(deleteNode->nodeData))->~RTTIPtr();
			break;
		case DatumType::Unknown:
			break;
		default:
			break;
		}
		// free data held by old node
		free(deleteNode->nodeData);
		// free the node itself
		free(deleteNode);
	}
	/// @brief FreeExternalNode
	/// Frees the headNode of an external Datum
	/// @param deleteNode: the node to free
	void Datum::FreeExternalNode(DatumNode* deleteNode)
	{
		if (!isExternal) { throw std::invalid_argument("You fool you absolute buffoon you called FreeExternalNode on an internal Datum"); }
		free(deleteNode);
	}
#pragma endregion
}