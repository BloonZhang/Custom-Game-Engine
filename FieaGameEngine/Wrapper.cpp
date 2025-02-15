#include "pch.h"
#include "Wrapper.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Wrapper);

/// @brief increment_depth
/// Increments the current depth of the Wrapper
void Wrapper::increment_depth()
{
	++mCurrentDepth;
	//if (mCurrentDepth > mMaxDepth) { mMaxDepth = mCurrentDepth; }
}

/// @brief decrement_depth
/// Decrements the current depth of the Wrapper
/// Asserts that it does not try to decrement 0 and underflow
void Fiea::GameEngine::Wrapper::decrement_depth()
{
	assert(mCurrentDepth != 0);
	--mCurrentDepth;
}

/// @brief get_current_depth
/// @return Returns the current depth
size_t Fiea::GameEngine::Wrapper::get_current_depth() const
{
	return mCurrentDepth;
}

/*
/// @brief get_max_depth
/// @return Returns the max depth
size_t Fiea::GameEngine::Wrapper::get_max_depth() const
{
	return mMaxDepth;
}
*/
