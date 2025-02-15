#pragma once

#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "ScopeWrapper.h"
#include "Scope.h"

#include "FactoryManagerSingleton.hpp"


namespace Fiea::GameEngine
{
	class ScopeHandler : public IParseHandler
	{
	public:
		//void initialize_handler() override;
		//void cleanup_handler() override;
		bool start_handler(Wrapper* wrapper, JsonPair jsonPair) override;
		bool end_handler(Wrapper* wrapper, JsonPair jsonPair) override;

		/*
		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;
		*/

	private:
		void AddDataToDatumAt(Json::Value, Datum*, DatumType, size_t);
	};
}