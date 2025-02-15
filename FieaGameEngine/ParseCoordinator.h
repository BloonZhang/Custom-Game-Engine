#pragma once

#include <fstream>

#include "Definitions.h"

#include "Wrapper.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	class ParseCoordinator
	{
	public:
		// Custom constructor
		ParseCoordinator(Wrapper*);
		~ParseCoordinator() = default;

		void add_handler(IParseHandler*);
		void remove_handler_by_address(const IParseHandler*);

		bool deserialize_json_string(const std::string&);
		bool deserialize_json_file(const std::string&);
		bool deserialize_json_stream(std::istream&);

		Wrapper* get_wrapper() const;
		void set_wrapper(Wrapper*);

		// TODO: DEBUG ONLY
		//ParseCoordinator() = default;

	private:
		// Delete rule of 5
		ParseCoordinator() = delete;
		ParseCoordinator(const ParseCoordinator&) = delete;
		ParseCoordinator(ParseCoordinator&&) = delete;
		ParseCoordinator& operator=(const ParseCoordinator&) = delete;
		ParseCoordinator& operator=(ParseCoordinator&&) = delete;

	private:
		// helper methods
		//void ParseJSONObject(Json::Value);
		bool ParseJSONObject(const Json::Value&);
		bool ParseSingleJSONPair(const JsonPair&);

	private:
		// member variables
		std::vector<IParseHandler*> mListOfHandlers;
		Wrapper* mWrapper;
		//Json::Value root;

	};
}