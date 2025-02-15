#include "pch.h"
#include <iostream>
#include "CppUnitTest.h"

#include "RTTI.h"
#include "TestClasses/Foo.h"
#include "TestClasses/Empty.h"
#include "TestClasses/ChildOfEmpty.h"

#include "TestDefinitions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
	TEST_CLASS(RTTITest)
	{
	public:
		// ported from FooTests.cpp
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}
		// ported from FooTests.cpp
		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTIPlaceholder)
		{
			Foo foo(5);
			Empty empty(5);
			ChildOfEmpty child(5);

			Assert::IsTrue(foo.Is(Foo::TypeIdClass()));
			Assert::IsTrue(empty.Is(Empty::TypeIdClass()));
			Assert::IsTrue(child.Is(Empty::TypeIdClass()));
			Assert::IsFalse(foo.Is(Empty::TypeIdClass()));
			Assert::IsFalse(empty.Is(ChildOfEmpty::TypeIdClass()));
		}

	private:
		// ported from FooTests.cpp
		inline static _CrtMemState _startMemState;
	};
}
