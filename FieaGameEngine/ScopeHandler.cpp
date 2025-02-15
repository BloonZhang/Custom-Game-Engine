#include "pch.h"
#include "ScopeHandler.h"

using namespace Fiea::GameEngine;

/// @brief start_handler
/// Starts handing a jsonPair and placing data into a wrapper.
/// Will return false if the wrapper or jsonPair cannot be handled.
/// Otherwise, will check what data type the Json::Value is, and appends it correspondingly to the scope
/// @param wrapper - A wrapper that will be treated as a ScopeWrapper that holds the Scope to populate 
/// @param jsonPair - The key/value pair to attempt to place inside the Scope
/// @return Returns true if successful
bool ScopeHandler::start_handler(Wrapper* wrapper, JsonPair jsonPair)
{
	// JSON grammar:
	// int: just an int
	// float: just a float
	// string: just a string
	// vec4: a string that starts with vec4
	// mat4: a string that starts with mat4

	// Check wrapper
	ScopeWrapper* scopeWrapper = wrapper->As<ScopeWrapper>();
	if (scopeWrapper == nullptr) { return false; }

	// Get scope that we're working with
	Scope* currentScope = scopeWrapper->get_current_scope();
	Datum* currentDatum; // don't assign yet, in case jsonPair is invalid.

	// Edge case: _class metadata. Just no-op and return true
	if (jsonPair.first == "_class") { return true; }

	// Check data type
	Json::Value jsonValue = jsonPair.second;
	// Check if it's an array
	if (jsonValue.isArray())
	{
		// Size check. If size is zero, then no handling needs to be done. return true
		if (jsonValue.size() == 0) { return true; }
		// int
		if (jsonValue[0].type() == Json::intValue || jsonValue[0].type() == Json::uintValue)
		{
			currentDatum = currentScope->add_datum(jsonPair.first);
			for (size_t index = 0; index < jsonValue.size(); ++index) 
			{ 
				AddDataToDatumAt(jsonValue[(Json::ArrayIndex)index], currentDatum, DatumType::Integer, index); 
			}
			return true;
		}
		// float
		else if (jsonValue[0].isDouble())
		{
			currentDatum = currentScope->add_datum(jsonPair.first);
			for (size_t index = 0; index < jsonValue.size(); ++index) 
			{ 
				AddDataToDatumAt(jsonValue[(Json::ArrayIndex)index], currentDatum, DatumType::Float, index); 
			}
			return true;
		}
		// vec4, mat4, or string
		// Note: does not have the "backup" case of converting to string if vec4( or mat4x4( syntax is wrong.
		// Instead, throws an exception
		else if (jsonValue[0].isString())
		{
			std::string jsonValueAsString = jsonValue[0].asString();
			// vec4
			if (StringStartsWith(jsonValueAsString, "vec4("))
			{
				currentDatum = currentScope->add_datum(jsonPair.first);
				for (size_t index = 0; index < jsonValue.size(); ++index) 
				{ 
					// try to convert it to a vec4
					try
					{
						glm::vec4 myVec = StringToVec4(jsonValue[(Json::ArrayIndex)index].asString());
					}
					catch (std::invalid_argument) 
					{ 
						throw std::invalid_argument("Invalid syntax in ScopeHandler::start_handler for a vec4 array.");
					}
					AddDataToDatumAt(jsonValue[(Json::ArrayIndex)index], currentDatum, DatumType::Vector4, index); 
				}
				return true;
			}
			// mat4
			else if (StringStartsWith(jsonValueAsString, "mat4x4("))
			{
				currentDatum = currentScope->add_datum(jsonPair.first);
				for (size_t index = 0; index < jsonValue.size(); ++index) 
				{ 
					// try to convert it to a vec4
					try
					{
						glm::mat4 myMat = StringToMat4(jsonValue[(Json::ArrayIndex)index].asString());
					}
					catch (std::invalid_argument) 
					{ 
						throw std::invalid_argument("Invalid syntax in ScopeHandler::start_handler for a vec4 array.");
					}
					AddDataToDatumAt(jsonValue[(Json::ArrayIndex)index], currentDatum, DatumType::Matrix4, index); 
				}
				return true;
			}
			// just normal string
			else
			{
				currentDatum = currentScope->add_datum(jsonPair.first);
				for (size_t index = 0; index < jsonValue.size(); ++index) 
				{ 
					AddDataToDatumAt(jsonValue[(Json::ArrayIndex)index], currentDatum, DatumType::String, index); 
				}
				return true;
			}
		}
		// array of objects: no-op. Let ParseCoordinator handles the recursive parsing
		else if (jsonValue[0].isObject())
		{
			/*
			for (size_t index = 0; index < jsonValue.size(); ++index)
			{
				// append a scope to the Wrapper. Next time we use the wrapper, we will use the appended wrapper (DFS logic)
				// We will create a special Scope if there is a JsonPair called _class inside the Object.
				Scope* newScope = nullptr;
				if (jsonValue.isMember("_class") && jsonValue["_class"].isString())
				{
					std::string className = jsonValue["_class"].asString();
					newScope = FactoryManagerSingleton::create_instance_of(className);
					// If class is nullptr, then we had a _class string that doesn't correspond with a class.
					if (newScope == nullptr) { throw std::invalid_argument("_class metadata in JSON had unrecognized className: " + className); }
					// TODO: is adopt_scope sufficient for this? beacuse adopt_scope calls push_back on the Datum, so if it already exists...
					currentScope->adopt_scope(newScope, jsonPair.first);
				}
				// Else we will just create a normal Scope
				else
				{
					newScope = currentScope->add_empty_scope(jsonPair.first);
				}
				scopeWrapper->set_current_scope(newScope);
				return true;
			}
			*/
			return true;
		}
		// Invalid
		else
		{
			return false;
		}
	}
	// Not array
	else
	{
		// int
		if (jsonValue.type() == Json::intValue || jsonValue.type() == Json::uintValue)
		{
			currentDatum = currentScope->add_datum(jsonPair.first);
			AddDataToDatumAt(jsonValue, currentDatum, DatumType::Integer, 0);
			return true;
		}
		// float
		else if (jsonValue.isDouble())
		{
			currentDatum = currentScope->add_datum(jsonPair.first);
			AddDataToDatumAt(jsonValue, currentDatum, DatumType::Float, 0);
			return true;
		}
		// vec4, mat4, or string
		else if (jsonValue.isString())
		{
			std::string jsonValueAsString = jsonValue.asString();
			bool stringHandled = false;
			// vec4
			if (StringStartsWith(jsonValueAsString, "vec4("))
			{
				// try to convert it to a vec4
				glm::vec4 myVec;
				try 
				{ 
					myVec = StringToVec4(jsonValueAsString); 
					stringHandled = true;
				}
				catch (std::invalid_argument) { stringHandled = false; }
				// If conversion was successful
				if (stringHandled)
				{
					currentDatum = currentScope->add_datum(jsonPair.first);
					AddDataToDatumAt(jsonValue, currentDatum, DatumType::Vector4, 0);
					return true;
				}
			}
			// mat4
			else if (StringStartsWith(jsonValueAsString, "mat4x4("))
			{
				// try to convert it to a mat4
				glm::mat4 myMat;
				try 
				{ 
					myMat = StringToMat4(jsonValueAsString); 
					stringHandled = true;
				}
				catch (std::invalid_argument) { stringHandled = false; }
				// If conversion was successful
				if (stringHandled)
				{
					currentDatum = currentScope->add_datum(jsonPair.first);
					AddDataToDatumAt(jsonValue, currentDatum, DatumType::Matrix4, 0);
					return true;
				}
			}
			// just normal string
			if (!stringHandled)
			{
				currentDatum = currentScope->add_datum(jsonPair.first);
				AddDataToDatumAt(jsonValue, currentDatum, DatumType::String, 0);
				return true;
			}
		}
		// another scope
		else if (jsonValue.isObject())
		{
			// append a scope to the Wrapper. Next time we use the wrapper, we will use the appended wrapper (DFS logic)
			// We will create a special Scope if there is a JsonPair called _class inside the Object.
			Scope* newScope = nullptr;
			if (jsonValue.isMember("_class") && jsonValue["_class"].isString())
			{
				std::string className = jsonValue["_class"].asString();
				newScope = FactoryManagerSingleton::create_instance_of(className);
				// If class is nullptr, then we had a _class string that doesn't correspond with a class.
				if (newScope == nullptr) { throw std::invalid_argument("_class metadata in JSON had unrecognized className: " + className); }
				// TODO: is adopt_scope sufficient for this? beacuse adopt_scope calls push_back on the Datum, so if it already exists...
				currentScope->adopt_scope(newScope, jsonPair.first);
			}
			// Else we will just create a normal Scope
			else
			{
				newScope = currentScope->add_empty_scope(jsonPair.first);
			}
			scopeWrapper->set_current_scope(newScope);
			return true;
		}
		// Invalid
		else
		{
			return false;
		}
	}

	return false;
}

/// @brief end_handler
/// Ends the handing of a key/value pair.
/// If the key/value pair was an Object, that means we are going one level higher in the Scope tree.
/// @param wrapper - A wrapper that will be treated as a ScopeWrapper that holds the Scope to populate 
/// @param jsonPair - The key/value pair to attempt to place inside the Scope
/// @return Returns true if successful
bool ScopeHandler::end_handler(Wrapper* wrapper, JsonPair jsonPair)
{
	// Check wrapper
	ScopeWrapper* scopeWrapper = wrapper->As<ScopeWrapper>();
	if (scopeWrapper == nullptr) { return false; }

	// If jsonPair is an object, then we go up one scope
	if (jsonPair.second.isObject())
	{
		Scope* currentScope = scopeWrapper->get_current_scope();
		if (currentScope == nullptr)
		{
			// Something bad happened
			throw std::out_of_range("Something bad hapepend in ScopeHandler::end_handler. Tried to end_handler when wrapper was nullptr");
			return false;
		}
		scopeWrapper->set_current_scope(currentScope->parent_scope());
	}
	return true;
}

/// @brief AddDataToDatumAt
/// Helper function to add data to a Datum
/// Will set_at if the index already exists. Else, will push_back
/// @param  jsonValue : the jsonValue to add to a DAtum
/// @param  currentDatum : the Datum to add to
/// @param  datumType : the expected type of the jsonValue
/// @param  index : the index to add the data to
void ScopeHandler::AddDataToDatumAt(Json::Value jsonValue, Datum* currentDatum, DatumType datumType, size_t index)
{
	switch (datumType)
	{
	case DatumType::Integer:
		if (currentDatum->size() == index) { currentDatum->push_int_back(jsonValue.asInt()); }
		else { currentDatum->set_int_at(index, jsonValue.asInt()); }
		break;
	case DatumType::Float:
		if (currentDatum->size() == index) { currentDatum->push_float_back((float)jsonValue.asDouble()); }
		else { currentDatum->set_float_at(index, (float)jsonValue.asDouble()); }
		break;
	case DatumType::Vector4:
		if (currentDatum->size() == index) { currentDatum->push_vec4_back(StringToVec4(jsonValue.asString())); }
		else { currentDatum->set_vec4_at(index, StringToVec4(jsonValue.asString())); }
		break;
	case DatumType::Matrix4:
		if (currentDatum->size() == index) { currentDatum->push_mat4_back(StringToMat4(jsonValue.asString())); }
		else { currentDatum->set_mat4_at(index, StringToMat4(jsonValue.asString())); }
		break;
	case DatumType::String:
		if (currentDatum->size() == index) { currentDatum->push_string_back(jsonValue.asString()); }
		else { currentDatum->set_string_at(index, jsonValue.asString()); }
		break;
	default:
		throw std::invalid_argument("Invalid DatumType passed to ScopeHander::AddDataToDatumAt");
	}
}
