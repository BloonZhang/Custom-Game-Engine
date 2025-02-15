#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "Wrapper.h"

namespace Fiea::GameEngine::test
{
	class IntWrapper : public Wrapper
	{
		RTTI_DECLARATIONS(TestWrapper, Wrapper);

	public:
		IntWrapper() = default;

		// this is the "output" for this wrapper
		inline int get_int() const { return mInt[0]; }
		inline std::vector<int> get_int_array() const { return mInt; }
		inline void add_int(int i) { mInt.push_back(i); }

		/*
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;
		*/

	private:
		std::vector<int> mInt;
	};
}