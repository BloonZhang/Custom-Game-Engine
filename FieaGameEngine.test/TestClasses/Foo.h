#pragma once

#include "RTTI.h"
#include "Definitions.h"

namespace Fiea::GameEngine::test
{

	class Foo : public RTTI {
		RTTI_DECLARATIONS(Foo, RTTI);

	public:
		// Debug code for testing datum
		Foo();
		Foo(const Foo&);
		Foo(int);
		~Foo() override;
		Foo& operator=(const Foo& rhs);
		bool operator==(const Foo& rhs) const;
	private:
		int* mHeapData;
	};

}