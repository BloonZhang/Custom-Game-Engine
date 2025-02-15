#include "pch.h"
#include "ParseCoordinator.h"

using namespace Fiea::GameEngine;

/// @brief ParseCoordinator(Wrapper*)
/// Constructs a ParseCoordinator with the specificed wrapper as the output.
/// Note: default construction has been deleted. ParseCoordinator must have a wrapper output
/// @param wrapperPtr - Pointer to the wrapper that the data will be output to
ParseCoordinator::ParseCoordinator(Wrapper* wrapperPtr) : mListOfHandlers(), mWrapper(wrapperPtr)
{
}

/// @brief add_handler
/// Adds a handler to the list of handlers of the ParseCoordinator.
/// Note that handlers are added in the order that they are called.
/// @param handlerPtr - Pointer to the handler that will be used for parsing
void ParseCoordinator::add_handler(IParseHandler* handlerPtr)
{
	mListOfHandlers.push_back(handlerPtr);
}

/// @brief remove_handler
/// Removes a handler from the list of handles of the ParseCoordinator.
/// Removes the handler by memory address. If the address has been changed, then the handler cannot be removed.
/// @param handlerPtr - Pointer to the handler to remove
void ParseCoordinator::remove_handler_by_address(const IParseHandler* handlerPtr)
{
	for (std::vector<IParseHandler*>::const_iterator it; it != mListOfHandlers.end(); ++it)
	{
		// Remove if addresses match
		if (*it == handlerPtr)
		{
			mListOfHandlers.erase(it);
			break;
		}
		// Should we assert that nothing was removed here?
		// throw std::invalid_argument("Handler not found in remove_handler_by_address.");
	}
}

/// @brief deserialize_json_string
/// Takes in a json String, and deserializes it into the Wrapper using the list of Handlers.
/// @param jsonString - A string representing a json tree
/// @return Returns true if deserialization was successful
bool ParseCoordinator::deserialize_json_string(const std::string& jsonString)
{
	// Try to read jsonString into a tree
	Json::Value root;
	std::istringstream ss(jsonString);
	try
	{
		ss >> root;
	}
	// If fails to read
	catch (...)
	{
		return false;
	}
	JsonPair rootPair = JsonPair("root"_s, root);

	// Initialize all handlers
	for (std::vector<IParseHandler*>::iterator::value_type it : mListOfHandlers)
	{
		it->initialize_handler();
	}

	// TODO: Debug only
	/*
	size_t result = root.size();
	bool testBool = root.isArray();
	testBool = root.isObject();
	size_t testSizet = root.size();
	Json::Value testValue = root["int"_s];
	Json::Value testCringe = root["cringe1"_s];
	testBool = testCringe.isObject();
	testBool = testCringe.isArray();
	testBool = testCringe[0].isObject();
	testBool = testCringe[0].isArray();
	int testInt = root.get("int"_s, 0).asInt();
	std::string testString = root.get("str"_s,"oh no"_s).asString();
	std::vector<std::string> testList = root.getMemberNames();
	*/
	// TODO: Debug only end

	// Parse the root
	bool handled = ParseJSONObject(root);

	// Cleanup all handlers
	for (std::vector<IParseHandler*>::iterator::value_type it : mListOfHandlers)
	{
		it->cleanup_handler();
	}

	// Return if success or not
	return handled;
}

/// @brief deserialize_json_file
/// Takes in the name of a file that contains only a json string, and deserializes that string.
/// @param filename - the name of the file that contains the json string
/// @return Returns true if deserialization was successful
bool ParseCoordinator::deserialize_json_file(const std::string& filename)
{
	// Create a stream for the file
	std::ifstream filestream(filename);
	// If failed to open
	if (!filestream.is_open()) { throw std::invalid_argument("Failed to open file in deserialize_json_file"); }

	// If success, the write it to a string
	std::stringstream ss;
	ss << filestream.rdbuf();
	// Then call deseriaized_json_string
	return deserialize_json_string(ss.str());
}

/// @brief deserialize_json_stream
/// Takes in an istream of a json string, and deserializes that stream
/// @param stream - the istream that contains the json string
/// @return Returns true if deserialization is successful
bool ParseCoordinator::deserialize_json_stream(std::istream& stream)
{
	std::stringstream ss;
	ss << stream.rdbuf();
	return deserialize_json_string(ss.str());
}

/// @brief get_wrapper
/// @return Returns a pointer to the current Wrapper
Wrapper* ParseCoordinator::get_wrapper() const
{
	return mWrapper;
}

/// @brief set_wrapper
/// @param wrapper - The wrapper to set
void ParseCoordinator::set_wrapper(Wrapper* wrapper)
{
	mWrapper = wrapper;
}


// Helper functions


/// @brief ParseJSONObject
/// Takes in a Json::Value that is guaranteed to be an object.
/// Goes through all pairs inside the object and calls ParseSingleJSONPair on the pair
/// Does not contain any logic regarding what the pairs inside the object are. That's for ParseSingleJSONPair to determine
/// @param objectValue - The object to parse
/// @return Returns if JSONObject was successfully parsed
bool ParseCoordinator::ParseJSONObject(const Json::Value& objectValue)
{
	// Get all keys
	std::vector<std::string> allKeysInObject = objectValue.getMemberNames();
	// For each key, parse it (recursively if needed)
	// Do not care if value is object, array, or just single value. ParseSingleJSONPair will take care of that
	Json::Value currentValue;
	for (const auto& it : allKeysInObject)
	{
		currentValue = objectValue[it];
		JsonPair currentPair = JsonPair(it, currentValue);
		bool handled = ParseSingleJSONPair(currentPair);
		// If it was not handled, then we weren't able to find a proper handler for this object
		if (!handled) { return false; }
	}
	// We successfully got through everything. Return true
	return true;
}

/// @brief ParseSingleJSONPair
/// Takes in a key and a Json::Value.
/// First goes through each handler in the list until one can handle it. If none of the handlers can handle it, then immediately returns false.
/// Next, checks if the Json::Value is an object. If so, recursively parse that object using ParseJSONObject
/// Finally, calls the end_handle of the handler used to handle it
/// @param jsonPair - The key/value pair to parse
/// @return Returns true if parsing was successful
bool ParseCoordinator::ParseSingleJSONPair(const JsonPair& jsonPair)
{
	// ------ STEP 1: Send to start_handler ------
	// Pass it to the Handlers and let them determine if they can do anything with it
	bool handled = false;
	std::vector<IParseHandler*>::iterator handlerThatHandled;
	for (handlerThatHandled = mListOfHandlers.begin(); handlerThatHandled != mListOfHandlers.end(); ++handlerThatHandled)
	{
		handled = (*handlerThatHandled)->start_handler(mWrapper, jsonPair);
		// If the handler can handle it, break
		if (handled)
		{
			break;
		}
	}
	// If it was not handled, then we weren't able to find a proper handler for this object
	if (!handled) { return false; }

	// ------ STEP 2: Check if recursion needs to happen ------
	// If jsonValue is an object, recursively parse.
	Json::Value jsonValue = jsonPair.second;
	if (jsonValue.isObject())
	{
		ParseJSONObject(jsonValue);
	}
	// If not an object, Check if array
	else
	{
		bool isArray = jsonValue.isArray();
		// If size of array is 0, then skip entire step
		if (jsonValue.size() != 0)
		{
			// Assert that jsonValue is not a multidimensional array
			if (isArray && jsonValue[0].isArray())
			{
				throw std::invalid_argument("Multidimensional arrays not supported in ParseSingleJSONPair.");
			}

			// If jsonValue is an array of objects, recursively parse all of them
			if (isArray && jsonValue[0].isObject())
			{
				// All Json::Values in this array must be an object. Do one for loop to make sure this is correct
				for (const auto& it : jsonValue)
				{
					assert(it.isObject()); /* ERROR: Not all values in object array were objects */
				}
				// At this point, it is confirmed that all values in the array are objects. 
				// Treat them as if they were thier own key/value pair
				// I kind of really hate this but I can't tell why and I'm not sure if it's going to be an issue
				for (Json::Value::ArrayIndex index = 0; index < jsonValue.size(); ++index)
				{
					JsonPair fakePair = JsonPair(jsonPair.first, jsonValue[index]);
					ParseSingleJSONPair(fakePair);
				}
			}
		}
	}

	// ------ STEP 3: Send to end_handler ------
	// Recursion finished. Tell handler to end parse
	(*handlerThatHandled)->end_handler(mWrapper, jsonPair);
	// We succesfully got through everything. Return true
	return true;
}
