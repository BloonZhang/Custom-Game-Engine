#pragma once

#include "RTTI.h"
#include "Definitions.h"

namespace Fiea::GameEngine::test
{
	class Empty : public RTTI
	{
		RTTI_DECLARATIONS(Empty, RTTI);
		
	public:
		Empty();
		Empty(const Empty&);
		Empty(int);
		~Empty() override;
		Empty& operator=(const Empty&);
		bool operator==(const Empty&) const;
	private:
		int* mHeapData;
	};
}