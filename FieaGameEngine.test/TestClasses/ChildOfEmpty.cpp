#include "pch.h"
#include "RTTI.h"
#include "Empty.h"
#include "ChildOfEmpty.h"

using namespace Fiea::GameEngine::test;

RTTI_DEFINITIONS(ChildOfEmpty);

ChildOfEmpty::ChildOfEmpty() : mHeapData(new int(0))
{
}

ChildOfEmpty::ChildOfEmpty(const ChildOfEmpty& rhs) : mHeapData(new int(*(rhs.mHeapData)))
{
}

ChildOfEmpty::ChildOfEmpty(int i) : mHeapData(new int(i))
{
}

ChildOfEmpty::~ChildOfEmpty()
{
	delete mHeapData;
}

ChildOfEmpty& ChildOfEmpty::operator=(const ChildOfEmpty& rhs)
{
	*(this->mHeapData) = *(rhs.mHeapData);
	return *this;
}

bool ChildOfEmpty::operator==(const ChildOfEmpty& rhs) const
{
	return *(this->mHeapData) == *(rhs.mHeapData);
}
