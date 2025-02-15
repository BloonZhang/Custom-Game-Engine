#pragma once

#include "Definitions.h"
#include "RTTI.h"

namespace Fiea::GameEngine
{
	class Wrapper : public RTTI
	{
		RTTI_DECLARATIONS(Wrapper, RTTI);

	public:
		virtual ~Wrapper() = default;
		void increment_depth();
		void decrement_depth();
		size_t get_current_depth() const;
		//size_t get_max_depth() const;

	protected:
		// Abstract class
		Wrapper() = default;

	private:
		size_t mCurrentDepth;
		//size_t mMaxDepth;
	};
}