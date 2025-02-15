/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "TestWrapper.h"

namespace Fiea::GameEngine::test
{
	class TestParseHandler final : public IParseHandler {
	public:
		/*
		class Wrapper : public ParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			//bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};
		*/

		void initialize_handler() override;
		void cleanup_handler() override;
		bool start_handler(Wrapper* wrapper, JsonPair jsonPair) override;
		bool end_handler(Wrapper* wrapper, JsonPair jsonPair) override;

		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}
