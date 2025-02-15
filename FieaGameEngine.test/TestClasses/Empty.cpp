#include "pch.h"
#include "Empty.h"

namespace Fiea::GameEngine::test
{
	RTTI_DEFINITIONS(Empty);

	Empty::Empty() : mHeapData(new int(0))
	{
	}

	Empty::Empty(const Empty& rhs) : mHeapData(new int(*(rhs.mHeapData)))
	{
	}

	Empty::Empty(int i) : mHeapData(new int(i))
	{
	}

	Empty::~Empty()
	{
		delete mHeapData;
	}

	Empty& Empty::operator=(const Empty& rhs)
	{
		*(this->mHeapData) = *(rhs.mHeapData);
		return *this;
	}

	bool Empty::operator==(const Empty& rhs) const
	{
		return *(this->mHeapData) == *(rhs.mHeapData);
	}

}