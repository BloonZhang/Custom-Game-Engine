#pragma once

//#include "ParseCoordinator.h" // because Wrapper is not a nested class
#include "IParseHandler.h"
#include "IntWrapper.h"

namespace Fiea::GameEngine::test
{
	class IntHandler : public IParseHandler
	{
	public:
		void initialize_handler() override;
		void cleanup_handler() override;
		bool start_handler(Wrapper* wrapper, JsonPair jsonPair) override;
		bool end_handler(Wrapper* wrapper, JsonPair jsonPair) override;

		/*
		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;
		*/

	private:
		bool CheckIfValid(Json::Value) const;
	};
}