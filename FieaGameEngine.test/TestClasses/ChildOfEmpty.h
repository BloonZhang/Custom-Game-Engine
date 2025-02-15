#pragma once

#include "RTTI.h"
#include "Empty.h"
#include "Definitions.h"

namespace Fiea::GameEngine::test
{
	class ChildOfEmpty : public Empty
	{
		RTTI_DECLARATIONS(ChildOfEmpty, Empty);

	public:
		ChildOfEmpty();
		ChildOfEmpty(const ChildOfEmpty&);
		ChildOfEmpty(int);
		~ChildOfEmpty() override;
		ChildOfEmpty& operator=(const ChildOfEmpty&);
		bool operator==(const ChildOfEmpty&) const;
	private:
		int* mHeapData;
	};
}