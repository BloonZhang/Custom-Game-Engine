#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine::test
{
#pragma region TestParseHandler::Wrapper
//	RTTI_DEFINITIONS(TestParseHandler::Wrapper);
//
//	/**
//	 * @brief RTTI override for Equals
//	 * @param rhs 
//	 * @return true if equal, false otherwise
//	*/
//	bool TestParseHandler::Wrapper::Equals(const RTTI* rhs) const {
//		if (rhs == nullptr) return false;
//
//		const TestParseHandler::Wrapper* other = rhs->As<TestParseHandler::Wrapper>();
//		return maxDepth == other->maxDepth;
//	}
//
//	/**
//	 * @brief RTTI override for ToString
//	 * @return string representation of this Wrapper
//	*/
//	std::string TestParseHandler::Wrapper::ToString() const {
//		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
//	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::initialize_handler() {
		initializeCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Cleanup
	*/
	void TestParseHandler::cleanup_handler() {
		cleanupCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Start
	 * @param unused
	 * @param unused
	 * @param wrapper The wrapper to populate 
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::start_handler(Wrapper* wrapper, JsonPair jsonPair) {
		TestWrapper* testWrapper = wrapper->As<TestWrapper>();
		if (testWrapper == nullptr) return false;

		testWrapper->increment_depth();

		if (testWrapper->get_current_depth() >= testWrapper->maxDepth) {
			testWrapper->maxDepth = testWrapper->get_current_depth();
		}
		++startCount;
		return true;
	}

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::end_handler(Wrapper* wrapper, JsonPair jsonPair) {
		Wrapper* testWrapper = wrapper->As<TestWrapper>();
		if (testWrapper == nullptr) return false;

		testWrapper->decrement_depth();

		++endCount;
		return true;
	}
#pragma endregion TestParseHandler
}