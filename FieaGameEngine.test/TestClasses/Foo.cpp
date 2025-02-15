#include "pch.h"
#include "Foo.h"
#include "RTTI.h"

using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(Foo);

Foo::Foo() : mHeapData(new int(0))
{
}

Foo::Foo(const Foo& rhs) : mHeapData(new int(*(rhs.mHeapData)))
{
}

Foo::Foo(int i) : mHeapData(new int(i))
{
}

Foo::~Foo()
{
	delete mHeapData;
}

Foo& Foo::operator=(const Foo& rhs)
{
	*(this->mHeapData) = *(rhs.mHeapData);
	return *this;
}

bool Foo::operator==(const Foo& rhs) const
{
	return *(this->mHeapData) == *(rhs.mHeapData);
}
