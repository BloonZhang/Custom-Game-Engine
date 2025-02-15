#include "pch.h"
#include "TestWrapper.h"

using namespace Fiea::GameEngine;
using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(TestWrapper);

/**
* @brief RTTI override for Equals
* @param rhs 
* @return true if equal, false otherwise
*/
bool TestWrapper::Equals(const RTTI* rhs) const {
	if (rhs == nullptr) return false;

	const TestWrapper* other = rhs->As<TestWrapper>();
	return maxDepth == other->maxDepth;
}

/**
* @brief RTTI override for ToString
* @return string representation of this Wrapper
*/
std::string TestWrapper::ToString() const {
	return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
}