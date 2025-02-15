#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "Wrapper.h"

namespace Fiea::GameEngine::test
{
	class TestWrapper : public Wrapper {
		RTTI_DECLARATIONS(TestWrapper, Wrapper);

	public:
		TestWrapper() = default;

		// this is the "output" for this wrapper
		std::size_t maxDepth{ 0 };

		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;
	};
}