#include "pch.h"
#include <iostream>
#include "CppUnitTest.h"
#include "glm/glm.hpp"
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/string_cast.hpp"

#include "Datum.h"
#include "Scope.h"
#include "TestClasses/Foo.h"
#include "TestDefinitions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
	// Commented out constexpr because weird warnings
	//constexpr static std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
	//constexpr static float operator"" _f(unsigned long long n) { return float(n); }
	static std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
	static float operator"" _f(unsigned long long n) { return float(n); }

	TEST_CLASS(DatumTest)
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

		TEST_METHOD(DefaultConstructor)
		{
			Datum myDatum;
			Assert::AreEqual(myDatum.get_type(), DatumType::Unknown);
			Assert::AreEqual(myDatum.size(), 0_z);
			Assert::IsTrue(myDatum.is_empty());

			Datum myDatum2;
			Assert::AreEqual(myDatum, myDatum2);
			Assert::AreNotSame(myDatum, myDatum2);
		}

		TEST_METHOD(CopyConstructor)
		{
			{
				Datum myDatum;
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 0_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Unknown);
			}
			{
				Datum myDatum;
				int myData = 0;
				myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum2.get_int_at(0), myData);
				Assert::AreEqual(myDatum2.get_int_at(1), myData);
				myDatum2.pop_back();
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_int_at(1), myData);
				myDatum.~Datum();
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum2.get_int_at(0), myData);
			}
			{
				Datum myDatum;
				float myData = 1_f;
				myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum2.get_float_at(0), myData);
				Assert::AreEqual(myDatum2.get_float_at(1), myData);
				myDatum2.pop_back();
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_float_at(1), myData);
				myDatum.~Datum();
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum2.get_float_at(0), myData);
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum2.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum2.get_vec4_at(1), myData);
				myDatum2.pop_back();
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData);
				myDatum.~Datum();
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum2.get_vec4_at(0), myData);
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum2.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum2.get_mat4_at(1), myData);
				myDatum2.pop_back();
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData);
				myDatum.~Datum();
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum2.get_mat4_at(0), myData);
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu okayu";
				myDatum.push_string_back(myData); myDatum.push_string_back(myData);
				Datum myDatum2(myDatum);
				Assert::AreEqual(myDatum2, myDatum);
				Assert::AreNotSame(myDatum2, myDatum);
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::String);
				Assert::AreEqual(myDatum2.get_string_at(0), myData);
				Assert::AreEqual(myDatum2.get_string_at(1), myData);
				myDatum2.pop_back();
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_string_at(1), myData);
				myDatum.~Datum();
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::String);
				Assert::AreEqual(myDatum2.get_string_at(0), myData);
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			{
				Datum myDatum;
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 0_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Unknown);
			}
			{
				Datum myDatum;
				int myData = 0;
				int myData2 = 10;
				myDatum.push_int_back(myData); myDatum.push_int_back(myData2);
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum2.get_int_at(0), myData);
				Assert::AreEqual(myDatum2.get_int_at(1), myData2);
			}
			{
				Datum myDatum;
				float myData = 0;
				float myData2 = 10;
				myDatum.push_float_back(myData); myDatum.push_float_back(myData2);
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum2.get_float_at(0), myData);
				Assert::AreEqual(myDatum2.get_float_at(1), myData2);
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData2);
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum2.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum2.get_vec4_at(1), myData2);
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData2);
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum2.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum2.get_mat4_at(1), myData2);
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				myDatum.push_string_back(myData); myDatum.push_string_back(myData2);
				Datum myDatum2(std::move(myDatum));
				Assert::AreEqual(myDatum2.size(), 2_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::String);
				Assert::AreEqual(myDatum2.get_string_at(0), myData);
				Assert::AreEqual(myDatum2.get_string_at(1), myData2);
			}
		}

		TEST_METHOD(AssignmentOperator)
		{
			{
				Datum defaultDatum;
				Datum intDatum; intDatum.push_int_back(int());
				Datum floatDatum; floatDatum.push_float_back(float());
				Datum vecDatum; vecDatum.push_vec4_back(glm::vec4());
				Datum matDatum; matDatum.push_mat4_back(glm::mat4());
				Datum strDatum; strDatum.push_string_back(std::string());
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum = defaultDatum; });
				Assert::ExpectException<std::invalid_argument>([&]() { floatDatum = defaultDatum; });
				Assert::ExpectException<std::invalid_argument>([&]() { vecDatum = defaultDatum; });
				Assert::ExpectException<std::invalid_argument>([&]() { matDatum = defaultDatum; });
				Assert::ExpectException<std::invalid_argument>([&]() { strDatum = defaultDatum; });
			}
			{
				Datum myDatum;
				Datum myDatum2;
				int myData = 0;
				myDatum2.push_int_back(myData);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				myDatum2.clear();
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				float myData = 0;
				myDatum2.push_float_back(myData);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				myDatum2.clear();
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::vec4 myData(0, 1, 2, 3);
				myDatum2.push_vec4_back(myData);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				myDatum2.clear();
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				myDatum2.push_mat4_back(myData);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				myDatum2.clear();
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				std::string myData = "mogu mogu okayu";
				myDatum2.push_string_back(myData);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				myDatum2.clear();
				myDatum = myDatum2;
				Assert::AreEqual(myDatum, myDatum2);
				Assert::AreNotSame(myDatum, myDatum2);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				Datum myDatum3;
				int myData = 0;
				myDatum3.push_int_back(myData);
				myDatum = myDatum2 = myDatum3;
				Assert::AreEqual(myDatum, myDatum3);
				Assert::AreNotSame(myDatum, myDatum3);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum2, myDatum3);
				Assert::AreNotSame(myDatum2, myDatum3);
				Assert::AreEqual(myDatum2.size(), 1_z);
				Assert::AreEqual(myDatum2.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum2.get_int_at(0), myData);
				myDatum = myDatum;
				Assert::AreEqual(myDatum, myDatum);
				Assert::AreSame(myDatum, myDatum);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
			}
		}

		TEST_METHOD(AssignmentOperatorScalar)
		{
			{
				Datum myDatum;
				int myData = 0;
				int myData2 = 1;
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = float(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::vec4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::mat4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = std::string(" "); });
			}
			{
				Datum myDatum;
				float myData = 0_f;
				float myData2 = 1_f;
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::vec4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::mat4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = std::string(" "); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = float(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::mat4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = std::string(" "); });
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = float(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::vec4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = std::string(" "); });
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				myDatum.push_string_back(myData); myDatum.push_string_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = float(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::vec4(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = glm::mat4(); });
			}
		}

		TEST_METHOD(ScalarConstructor)
		{
			{
				int myData = 1;
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
			}
			{
				float myData = 1_f;
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
			}
			{
				std::string myData = "mogu mogu okayu";
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
			}
		}

		TEST_METHOD(MoveOperator)
		{
			{
				Datum defaultDatum;
				Datum intDatum; intDatum.push_int_back(int());
				/*
				Datum floatDatum; floatDatum.push_float_back(float());
				Datum vecDatum; vecDatum.push_vec4_back(glm::vec4());
				Datum matDatum; matDatum.push_mat4_back(glm::mat4());
				Datum strDatum; strDatum.push_string_back(std::string());
				*/
				// If move operator is called on mismatching types, just return this.
				// TODO: is there a better way to indicate that it failed?
				//Assert::AreEqual(intDatum, intDatum = std::move(defaultDatum));
				/*
				floatDatum = defaultDatum;
				vecDatum = defaultDatum;
				matDatum = defaultDatum;
				strDatum = defaultDatum;
				*/
			}
			{
				Datum myDatum;
				Datum myDatum2;
				int myData = 0;
				myDatum2.push_int_back(myData);
				myDatum = std::move(myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				float myData = 1_f;
				myDatum2.push_float_back(myData);
				myDatum = std::move(myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::vec4 myData(0, 1, 2, 3);
				myDatum2.push_vec4_back(myData);
				myDatum = std::move(myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				myDatum2.push_mat4_back(myData);
				myDatum = std::move(myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				std::string myData = "mogu mogu okayu";
				myDatum2.push_string_back(myData);
				myDatum = std::move(myDatum2);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				Datum myDatum3;
				int myData = 0;
				myDatum3.push_int_back(myData);
				myDatum = std::move(myDatum2) = std::move(myDatum3);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				myDatum = std::move(myDatum);
				Assert::AreEqual(myDatum, myDatum); // Intentional warning C26800: Use of a moved from object: ''myDatum'' (lifetime.1).
				Assert::AreSame(myDatum, myDatum);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
			}
		}

		TEST_METHOD(PushFront)
		{
			{
				int myData = 1;
				int myData2 = 2;
				Datum myDatum;
				myDatum.push_int_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreNotSame(myDatum.get_int_at(0), myData);
				myDatum.push_int_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				Assert::AreEqual(myDatum.get_int_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_front(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_front(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_front(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_front(std::string(" ")); });
			}
			{
				float myData = 1_f;
				float myData2 = 2_f;
				Datum myDatum;
				myDatum.push_float_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreNotSame(myDatum.get_float_at(0), myData);
				myDatum.push_float_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				Assert::AreEqual(myDatum.get_float_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_front(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_front(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_front(std::string(" ")); });
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				Datum myDatum;
				myDatum.push_vec4_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreNotSame(myDatum.get_vec4_at(0), myData);
				myDatum.push_vec4_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_front(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_front(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_front(std::string(" ")); });
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				Datum myDatum;
				myDatum.push_mat4_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreNotSame(myDatum.get_mat4_at(0), myData);
				myDatum.push_mat4_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_front(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_front(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_front(std::string(" ")); });
			}
			{
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				Datum myDatum;
				myDatum.push_string_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreNotSame(myDatum.get_string_at(0), myData);
				myDatum.push_string_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData2);
				Assert::AreEqual(myDatum.get_string_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_front(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_front(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_front(glm::mat4()); });
			}
		}

		TEST_METHOD(PushFrontStringRepresentation)
		{
			{
				int myData = 1;
				std::string myDataString = std::to_string(myData);
				int myData2 = 2;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_int_front(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreNotSame(myDatum.get_int_at(0), myData);
				myDatum.push_int_front(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				Assert::AreEqual(myDatum.get_int_at(1), myData);
			}
			{
				float myData = 1_f;
				std::string myDataString = std::to_string(myData);
				float myData2 = 2_f;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_float_front(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreNotSame(myDatum.get_float_at(0), myData);
				myDatum.push_float_front(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				Assert::AreEqual(myDatum.get_float_at(1), myData);
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				std::string myDataString = glm::to_string(myData);
				glm::vec4 myData2(10, 11, 12, 13);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_vec4_front(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreNotSame(myDatum.get_vec4_at(0), myData);
				myDatum.push_vec4_front(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData);
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				std::string myDataString = glm::to_string(myData);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_mat4_front(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreNotSame(myDatum.get_mat4_at(0), myData);
				myDatum.push_mat4_front(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData);
			}
		}

		TEST_METHOD(PushBack)
		{
			{
				int myData = 1;
				int myData2 = 2;
				Datum myDatum;
				myDatum.push_int_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreNotSame(myDatum.get_int_at(0), myData);
				myDatum.push_int_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_back(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_back(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_back(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				float myData = 1_f;
				float myData2 = 2_f;
				Datum myDatum;
				myDatum.push_float_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreNotSame(myDatum.get_float_at(0), myData);
				myDatum.push_float_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_back(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_back(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::vec4 myData = glm::vec4(0, 1, 2, 3);
				glm::vec4 myData2 = glm::vec4(10, 11, 12, 13);
				Datum myDatum;
				myDatum.push_vec4_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreNotSame(myDatum.get_vec4_at(0), myData);
				myDatum.push_vec4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_back(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_back(glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				Datum myDatum;
				myDatum.push_mat4_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreNotSame(myDatum.get_mat4_at(0), myData);
				myDatum.push_mat4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_back(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_back(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				Datum myDatum;
				myDatum.push_string_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreNotSame(myDatum.get_string_at(0), myData);
				myDatum.push_string_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreEqual(myDatum.get_string_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_float_back(0_f); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_vec4_back(glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_mat4_back(glm::mat4()); });
			}
		}

		TEST_METHOD(PushBackStringRepresentation)
		{
			{
				int myData = 1;
				std::string myDataString = std::to_string(myData);
				int myData2 = 2;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_int_back(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreNotSame(myDatum.get_int_at(0), myData);
				myDatum.push_int_back(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
			}
			{
				float myData = 1_f;
				std::string myDataString = std::to_string(myData);
				float myData2 = 2_f;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_float_back(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreNotSame(myDatum.get_float_at(0), myData);
				myDatum.push_float_back(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				std::string myDataString = glm::to_string(myData);
				glm::vec4 myData2(10, 11, 12, 13);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_vec4_back(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreNotSame(myDatum.get_vec4_at(0), myData);
				myDatum.push_vec4_back(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				std::string myDataString = glm::to_string(myData);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_mat4_back(myDataString);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreNotSame(myDatum.get_mat4_at(0), myData);
				myDatum.push_mat4_back(myData2String);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
			}
		}

		TEST_METHOD(Get)
		{
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				int myData = 0;
				int myData2 = 1;
				myDatum.push_int_back(myData);
				myDatum.push_int_back(myData2);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_int_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_string_at(0); });
				Assert::AreEqual(myConstDatum.get_int_at(0), myData);
				Assert::AreEqual(myConstDatum.get_int_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([myConstDatum]() { myConstDatum.get_int_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_string_at(0); });
			}
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				float myData = 0_f;
				float myData2 = 1_f;
				myDatum.push_float_back(myData);
				myDatum.push_float_back(myData2);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.get_float_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_string_at(0); });
				Assert::AreEqual(myConstDatum.get_float_at(0), myData);
				Assert::AreEqual(myConstDatum.get_float_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_float_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_string_at(0); });
			}
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				myDatum.push_vec4_back(myData);
				myDatum.push_vec4_back(myData2);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_vec4_at(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_string_at(0); });
				Assert::AreEqual(myConstDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myConstDatum.get_vec4_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_vec4_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_mat4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_string_at(0); });
			}
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				myDatum.push_mat4_back(myData);
				myDatum.push_mat4_back(myData2);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_mat4_at(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_string_at(0); });
				Assert::AreEqual(myConstDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myConstDatum.get_mat4_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_mat4_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_string_at(0); });
			}
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				myDatum.push_string_back(myData);
				myDatum.push_string_back(myData2);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreEqual(myDatum.get_string_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_string_at(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_mat4_at(0); });
				Assert::AreEqual(myConstDatum.get_string_at(0), myData);
				Assert::AreEqual(myConstDatum.get_string_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_string_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_float_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_vec4_at(0); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_mat4_at(0); });
			}
		}

		TEST_METHOD(GetAsString)
		{
			{
				int myData = 1;
				std::string myDataString = std::to_string(myData);
				int myData2 = 2;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_int_back(myData);
				myDatum.push_int_back(myData2);
				Assert::AreEqual(myDatum.get_int_at_as_string(0), myDataString);
				Assert::AreEqual(myDatum.get_int_at_as_string(1), myData2String);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_int_at_as_string(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_vec4_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_mat4_at_as_string(0); });
			}
			{
				float myData = 1_f;
				std::string myDataString = std::to_string(myData);
				float myData2 = 2_f;
				std::string myData2String = std::to_string(myData2);
				Datum myDatum;
				myDatum.push_float_back(myData);
				myDatum.push_float_back(myData2);
				Assert::AreEqual(myDatum.get_float_at_as_string(0), myDataString);
				Assert::AreEqual(myDatum.get_float_at_as_string(1), myData2String);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_float_at_as_string(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_vec4_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_mat4_at_as_string(0); });
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				std::string myDataString = glm::to_string(myData);
				glm::vec4 myData2(10, 11, 12, 13);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_vec4_back(myData);
				myDatum.push_vec4_back(myData2);
				Assert::AreEqual(myDatum.get_vec4_at_as_string(0), myDataString);
				Assert::AreEqual(myDatum.get_vec4_at_as_string(1), myData2String);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_vec4_at_as_string(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_mat4_at_as_string(0); });
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				std::string myDataString = glm::to_string(myData);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				std::string myData2String = glm::to_string(myData2);
				Datum myDatum;
				myDatum.push_mat4_back(myData);
				myDatum.push_mat4_back(myData2);
				Assert::AreEqual(myDatum.get_mat4_at_as_string(0), myDataString);
				Assert::AreEqual(myDatum.get_mat4_at_as_string(1), myData2String);
				Assert::ExpectException<std::out_of_range>([myDatum]() { myDatum.get_mat4_at_as_string(3); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_int_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_float_at_as_string(0); });
				Assert::ExpectException<std::invalid_argument>([myDatum]() { myDatum.get_vec4_at_as_string(0); });
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum myDatum;
				int myData = 0;
				int myData2 = 1;
				myDatum.push_int_back(myData); myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				myDatum.set_int_at(1, myData2);
				int& temp = myDatum.set_int_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
				Assert::AreEqual(myDatum.get_int_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_int_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_float_at(0, float()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_vec4_at(0, glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_mat4_at(0, glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_string_at(0, std::string()); });
			}
			{
				Datum myDatum;
				float myData = 0_f;
				float myData2 = 1_f;
				myDatum.push_float_back(myData); myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				myDatum.set_float_at(1, myData2);
				float& temp = myDatum.set_float_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
				Assert::AreEqual(myDatum.get_float_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_float_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_vec4_at(0, glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_mat4_at(0, glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_string_at(0, std::string()); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				myDatum.set_vec4_at(1, myData2);
				glm::vec4& temp = myDatum.set_vec4_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_vec4_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_float_at(0, float()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_mat4_at(0, glm::mat4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_string_at(0, std::string()); });
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				myDatum.set_mat4_at(1, myData2);
				glm::mat4& temp = myDatum.set_mat4_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_mat4_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_float_at(0, float()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_vec4_at(0, glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_string_at(0, std::string()); });
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				myDatum.push_string_back(myData); myDatum.push_string_back(myData); myDatum.push_string_back(myData);
				myDatum.set_string_at(1, myData2);
				std::string& temp = myDatum.set_string_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreEqual(myDatum.get_string_at(1), myData2);
				Assert::AreEqual(myDatum.get_string_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_string_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_float_at(0, float()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_vec4_at(0, glm::vec4()); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_mat4_at(0, glm::mat4()); });
			}
		}

		TEST_METHOD(SetAsString)
		{
			{
				Datum myDatum;
				int myData = 0;
				int myData2 = 1;
				std::string myData2String = std::to_string(myData2);
				myDatum.push_int_back(myData); myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				myDatum.set_int_at_using_string(1, myData2String);
				int& temp = myDatum.set_int_at_using_string(2, myData2String); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
				Assert::AreEqual(myDatum.get_int_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_int_at_using_string(3, myData2String); });
				Datum floatDatum; floatDatum.push_float_back(1_f);
				Assert::ExpectException<std::invalid_argument>([&]() { floatDatum.set_int_at_using_string(0, myData2String); });
			}
			{
				Datum myDatum;
				float myData = 0_f;
				float myData2 = 1_f;
				std::string myData2String = std::to_string(myData2);
				myDatum.push_float_back(myData); myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				myDatum.set_float_at_using_string(1, myData2String);
				float& temp = myDatum.set_float_at_using_string(2, myData2String); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
				Assert::AreEqual(myDatum.get_float_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_float_at_using_string(3, myData2String); });
				Datum intDatum; intDatum.push_int_back(1);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_float_at_using_string(0, myData2String); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				std::string myData2String = glm::to_string(myData2);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				myDatum.set_vec4_at_using_string(1, myData2String);
				glm::vec4& temp = myDatum.set_vec4_at_using_string(2, myData2String); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_vec4_at_using_string(3, myData2String); });
				Datum intDatum; intDatum.push_int_back(1);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_vec4_at_using_string(0, myData2String); });
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				std::string myData2String = glm::to_string(myData2);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				myDatum.set_mat4_at_using_string(1, myData2String);
				glm::mat4& temp = myDatum.set_mat4_at_using_string(2, myData2String); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_mat4_at_using_string(3, myData2String); });
				Datum intDatum; intDatum.push_int_back(1);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_mat4_at_using_string(0, myData2String); });
			}
		}

		TEST_METHOD(SetSizeTest)
		{
			{
				Datum myDatum;
				int myData = 1;
				int defaultData = int();
				myDatum.push_int_back(myData); myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				myDatum.set_size(3);
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				myDatum.set_size(6);
				Assert::AreEqual(myDatum.size(), 6_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(5), defaultData);
				myDatum.set_size(0);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
			}
			{
				Datum myDatum;
				float myData = 1_f;
				float defaultData = float();
				myDatum.push_float_back(myData); myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				myDatum.set_size(3);
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				myDatum.set_size(6);
				Assert::AreEqual(myDatum.size(), 6_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(5), defaultData);
				myDatum.set_size(0);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 defaultData = glm::vec4();
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				myDatum.set_size(3);
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				myDatum.set_size(6);
				Assert::AreEqual(myDatum.size(), 6_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(5), defaultData);
				myDatum.set_size(0);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
			}
			{
				Datum myDatum;
				glm::mat4 myData(0, 1, 2, 3, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33);
				glm::mat4 defaultData = glm::mat4();
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				myDatum.set_size(3);
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				myDatum.set_size(6);
				Assert::AreEqual(myDatum.size(), 6_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(5), defaultData);
				myDatum.set_size(0);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu okayu";
				std::string defaultData = std::string();
				myDatum.push_string_back(myData); myDatum.push_string_back(myData); myDatum.push_string_back(myData);
				myDatum.set_size(3);
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				myDatum.set_size(6);
				Assert::AreEqual(myDatum.size(), 6_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreEqual(myDatum.get_string_at(5), defaultData);
				myDatum.set_size(0);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
			}
		}

		TEST_METHOD(Clear)
		{
			{
				Datum myDatum;
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::Unknown);
				Assert::AreEqual(myDatum.size(), 0_z);
			}
			{
				Datum myDatum;
				int myData = 1;
				myDatum.push_int_back(myData); myDatum.push_int_back(myData); myDatum.push_int_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				Datum myDatum;
				float myData = 1_f;
				myDatum.push_float_back(myData); myDatum.push_float_back(myData); myDatum.push_float_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData); myDatum.push_vec4_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				Datum myDatum;
				glm::mat4 myData(0, 1, 2, 3, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33);
				myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData); myDatum.push_mat4_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				Datum myDatum;
				std::string myData("mogu mogu okayu");
				myDatum.push_string_back(myData); myDatum.push_string_back(myData); myDatum.push_string_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum.clear();
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
			}
		}

		TEST_METHOD(PopFront)
		{
			{
				Datum myDatum;
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_front(); });
			}
			{
				int myData = 1;
				int myData2 = 2;
				Datum myDatum;
				myDatum.push_int_back(myData);
				myDatum.push_int_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_int_at(1); });
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_int_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				float myData = 1_f;
				float myData2 = 2_f;
				Datum myDatum;
				myDatum.push_float_back(myData);
				myDatum.push_float_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_float_at(1); });
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_float_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				Datum myDatum;
				myDatum.push_vec4_back(myData);
				myDatum.push_vec4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_vec4_at(1); });
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				Datum myDatum;
				myDatum.push_mat4_back(myData);
				myDatum.push_mat4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_mat4_at(1); });
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_mat4_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				Datum myDatum;
				myDatum.push_string_back(myData);
				myDatum.push_string_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_string_at(1); });
				myDatum.pop_front();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_string_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
			}
		}

		TEST_METHOD(PopBack)
		{
			{
				Datum myDatum;
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
			}
			{
				int myData = 1;
				int myData2 = 2;
				Datum myDatum;
				myDatum.push_int_back(myData);
				myDatum.push_int_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_int_at(1); });
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_int_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				float myData = 1_f;
				float myData2 = 2_f;
				Datum myDatum;
				myDatum.push_float_back(myData);
				myDatum.push_float_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_float_at(1); });
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_float_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				Datum myDatum;
				myDatum.push_vec4_back(myData);
				myDatum.push_vec4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_vec4_at(1); });
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_vec4_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				Datum myDatum;
				myDatum.push_mat4_back(myData);
				myDatum.push_mat4_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_mat4_at(1); });
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_mat4_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_string_back(std::string(" ")); });
			}
			{
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				Datum myDatum;
				myDatum.push_string_back(myData);
				myDatum.push_string_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_string_at(1); });
				myDatum.pop_back();
				Assert::AreEqual(myDatum.size(), 0_z);
				Assert::ExpectException<std::out_of_range>([&myDatum]() {myDatum.get_string_at(0); });
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.pop_back(); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
			}
		}

		TEST_METHOD(EqualityOperator)
		{
			{
				Datum myDatum;
				Datum myDatum2;
				int myData = 1;
				int myData2 = 2;
				myDatum.push_int_back(myData);
				Assert::IsTrue(myDatum != myDatum2);
				myDatum2.push_int_back(myData);
				Assert::IsTrue(myDatum == myDatum2);
				myDatum2.clear(); myDatum2.push_int_back(myData2);
				Assert::IsTrue(myDatum != myDatum2);
			}
		}

		TEST_METHOD(EqualityOperatorScalar)
		{
			{
				Datum myDatum;
				int myData = 1;
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				myDatum.push_int_back(myData);
				Assert::IsTrue(myDatum == myData);
				Assert::IsTrue(myData == myDatum);
				Assert::IsFalse(myDatum != myData);
				Assert::IsFalse(myData != myDatum);
				myDatum.push_int_back(myData);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				Datum myDatum2;
				float myData2 = float(myData);
				myDatum2.push_float_back(myData2);
				Assert::IsFalse(myDatum2 == myData);
				Assert::IsFalse(myData == myDatum2);
				Assert::IsTrue(myDatum2 != myData);
				Assert::IsTrue(myData != myDatum2);
			}
			{
				Datum myDatum;
				float myData = 1;
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				myDatum.push_float_back(myData);
				Assert::IsTrue(myDatum == myData);
				Assert::IsTrue(myData == myDatum);
				Assert::IsFalse(myDatum != myData);
				Assert::IsFalse(myData != myDatum);
				myDatum.push_float_back(myData);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				Datum myDatum2;
				int myData2 = int(myData);
				myDatum2.push_int_back(myData2);
				Assert::IsFalse(myDatum2 == myData);
				Assert::IsFalse(myData == myDatum2);
				Assert::IsTrue(myDatum2 != myData);
				Assert::IsTrue(myData != myDatum2);
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				myDatum.push_vec4_back(myData);
				Assert::IsTrue(myDatum == myData);
				Assert::IsTrue(myData == myDatum);
				Assert::IsFalse(myDatum != myData);
				Assert::IsFalse(myData != myDatum);
				myDatum.push_vec4_back(myData);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				Datum myDatum2;
				float myData2 = 1_f;
				myDatum2.push_float_back(myData2);
				Assert::IsFalse(myDatum2 == myData);
				Assert::IsFalse(myData == myDatum2);
				Assert::IsTrue(myDatum2 != myData);
				Assert::IsTrue(myData != myDatum2);
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				myDatum.push_mat4_back(myData);
				Assert::IsTrue(myDatum == myData);
				Assert::IsTrue(myData == myDatum);
				Assert::IsFalse(myDatum != myData);
				Assert::IsFalse(myData != myDatum);
				myDatum.push_mat4_back(myData);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				Datum myDatum2;
				float myData2 = 1_f;
				myDatum2.push_float_back(myData2);
				Assert::IsFalse(myDatum2 == myData);
				Assert::IsFalse(myData == myDatum2);
				Assert::IsTrue(myDatum2 != myData);
				Assert::IsTrue(myData != myDatum2);
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu okayu";
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				myDatum.push_string_back(myData);
				Assert::IsTrue(myDatum == myData);
				Assert::IsTrue(myData == myDatum);
				Assert::IsFalse(myDatum != myData);
				Assert::IsFalse(myData != myDatum);
				myDatum.push_string_back(myData);
				Assert::IsFalse(myDatum == myData);
				Assert::IsFalse(myData == myDatum);
				Assert::IsTrue(myDatum != myData);
				Assert::IsTrue(myData != myDatum);
				Datum myDatum2;
				float myData2 = 1_f;
				myDatum2.push_float_back(myData2);
				Assert::IsFalse(myDatum2 == myData);
				Assert::IsFalse(myData == myDatum2);
				Assert::IsTrue(myDatum2 != myData);
				Assert::IsTrue(myData != myDatum2);
			}
		}

		TEST_METHOD(ExternalMemory)
		{
			{
				Datum myDatum;
				Datum myDatum2;
				int myData = 1;
				int myData2 = 2;
				int newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				int newMemory2[5]{ myData2, myData2, myData2, myData2, myData2 };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_int_storage(newMemory, newMemorySize);
				myDatum2.set_external_int_storage(newMemory2, newMemory2Size);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				const Datum& myConstDatum = myDatum;
				Assert::AreEqual(myConstDatum.size(), newMemory2Size);
				Assert::AreEqual(myConstDatum.get_int_at(0), myData2);
				
				Datum myDatum3(myDatum2);
				Assert::AreEqual(myDatum3.size(), newMemory2Size);
				Assert::AreEqual(myDatum3.get_int_at(0), myData2);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				float myData = 1_f;
				float myData2 = 2_f;
				float newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				float newMemory2[5]{ myData2, myData2, myData2, myData2, myData2 };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_float_storage(newMemory, newMemorySize);
				myDatum2.set_external_float_storage(newMemory2, newMemory2Size);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				const Datum& myConstDatum = myDatum;
				Assert::AreEqual(myConstDatum.size(), newMemory2Size);
				Assert::AreEqual(myConstDatum.get_float_at(0), myData2);

				Datum myDatum3(myDatum2);
				Assert::AreEqual(myDatum3.size(), newMemory2Size);
				Assert::AreEqual(myDatum3.get_float_at(0), myData2);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(0, 1, 2, 3);
				glm::vec4 newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				glm::vec4 newMemory2[5]{ myData2, myData2, myData2, myData2, myData2 };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_vec4_storage(newMemory, newMemorySize);
				myDatum2.set_external_vec4_storage(newMemory2, newMemory2Size);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				const Datum& myConstDatum = myDatum;
				Assert::AreEqual(myConstDatum.size(), newMemory2Size);
				Assert::AreEqual(myConstDatum.get_vec4_at(0), myData2);

				Datum myDatum3(myDatum2);
				Assert::AreEqual(myDatum3.size(), newMemory2Size);
				Assert::AreEqual(myDatum3.get_vec4_at(0), myData2);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				glm::mat4 newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				glm::mat4 newMemory2[5]{ myData2, myData2, myData2, myData2, myData2 };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_mat4_storage(newMemory, newMemorySize);
				myDatum2.set_external_mat4_storage(newMemory2, newMemory2Size);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				const Datum& myConstDatum = myDatum;
				Assert::AreEqual(myConstDatum.size(), newMemory2Size);
				Assert::AreEqual(myConstDatum.get_mat4_at(0), myData2);

				Datum myDatum3(myDatum2);
				Assert::AreEqual(myDatum3.size(), newMemory2Size);
				Assert::AreEqual(myDatum3.get_mat4_at(0), myData2);
			}
			{
				Datum myDatum;
				Datum myDatum2;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				std::string newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				std::string newMemory2[5]{ myData2, myData2, myData2, myData2, myData2 };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_string_storage(newMemory, newMemorySize);
				myDatum2.set_external_string_storage(newMemory2, newMemory2Size);
				myDatum = myDatum2;
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_string_at(0), myData2);
				const Datum& myConstDatum = myDatum;
				Assert::AreEqual(myConstDatum.size(), newMemory2Size);
				Assert::AreEqual(myConstDatum.get_string_at(0), myData2);

				Datum myDatum3(myDatum2);
				Assert::AreEqual(myDatum3.size(), newMemory2Size);
				Assert::AreEqual(myDatum3.get_string_at(0), myData2);
			}
			{
				Datum myDatum;
				int myData = 1;
				std::string myDataString = std::to_string(myData);
				int myData2 = 2;
				int newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				int newMemory2[5]{ myData2, myData, myData, myData, myData };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_int_storage(newMemory, newMemorySize);
				Assert::AreEqual(myDatum.size(), newMemorySize);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				myDatum.set_external_int_storage(newMemory2, newMemory2Size);
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_type(), DatumType::Integer);
				Assert::AreEqual(myDatum.get_int_at(0), myData2);
				Assert::AreEqual(myDatum.get_int_at(4), myData);
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum = myData; });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_int_front(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_int_back(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_int_front(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_int_back(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_front(); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_back(); });
			}
			{
				Datum myDatum;
				float myData = 1_f;
				std::string myDataString = std::to_string(myData);
				float myData2 = 2_f;
				float newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				float newMemory2[5]{ myData2, myData, myData, myData, myData };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_float_storage(newMemory, newMemorySize);
				Assert::AreEqual(myDatum.size(), newMemorySize);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				myDatum.set_external_float_storage(newMemory2, newMemory2Size);
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_type(), DatumType::Float);
				Assert::AreEqual(myDatum.get_float_at(0), myData2);
				Assert::AreEqual(myDatum.get_float_at(4), myData);
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum = myData; });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_float_front(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_float_back(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_float_front(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_float_back(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_front(); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_back(); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				std::string myDataString = glm::to_string(myData);
				glm::vec4 myData2(10, 11, 12, 13);
				glm::vec4 newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				glm::vec4 newMemory2[5]{ myData2, myData, myData, myData, myData };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_vec4_storage(newMemory, newMemorySize);
				Assert::AreEqual(myDatum.size(), newMemorySize);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				myDatum.set_external_vec4_storage(newMemory2, newMemory2Size);
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_type(), DatumType::Vector4);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(4), myData);
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum = myData; });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_vec4_front(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_vec4_back(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_vec4_front(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_vec4_back(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_front(); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_back(); });
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				std::string myDataString = glm::to_string(myData);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				glm::mat4 newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				glm::mat4 newMemory2[5]{ myData2, myData, myData, myData, myData };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_mat4_storage(newMemory, newMemorySize);
				Assert::AreEqual(myDatum.size(), newMemorySize);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				myDatum.set_external_mat4_storage(newMemory2, newMemory2Size);
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_type(), DatumType::Matrix4);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(4), myData);
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum = myData; });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_mat4_front(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_mat4_back(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_mat4_front(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_mat4_back(myDataString); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_front(); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_back(); });
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				std::string newMemory[1]{ myData };
				size_t newMemorySize = 1_z;
				std::string newMemory2[5]{ myData2, myData, myData, myData, myData };
				size_t newMemory2Size = 5_z;
				myDatum.set_external_string_storage(newMemory, newMemorySize);
				Assert::AreEqual(myDatum.size(), newMemorySize);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				myDatum.set_external_string_storage(newMemory2, newMemory2Size);
				Assert::AreEqual(myDatum.size(), newMemory2Size);
				Assert::AreEqual(myDatum.get_type(), DatumType::String);
				Assert::AreEqual(myDatum.get_string_at(0), myData2);
				Assert::AreEqual(myDatum.get_string_at(4), myData);
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum = myData; });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_string_front(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.push_string_back(myData); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_front(); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.pop_back(); });
			}
		}

		TEST_METHOD(ExternalMemorySet)
		{
			{
				Datum myDatum;
				int myData = 1;
				int myData2 = 2;
				int newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_int_storage(newMemory, newMemorySize);
				myDatum.set_int_at(1, myData2);
				Assert::AreEqual(myDatum.get_int_at(0), myData);
				Assert::AreEqual(myDatum.get_int_at(1), myData2);
				Assert::AreEqual(myDatum.get_int_at(2), myData);
				Assert::AreEqual(newMemory[1], myData2);

				Datum myDatum2;
				int myData3 = 3;
				myDatum2.set_external_void_storage(newMemory, newMemorySize, DatumType::Integer);
				myDatum2.set_int_at(1, myData3);
				Assert::AreEqual(myDatum2.get_int_at(0), myData);
				Assert::AreEqual(myDatum2.get_int_at(1), myData3);
				Assert::AreEqual(myDatum2.get_int_at(2), myData);
				Assert::AreEqual(newMemory[1], myData3);

				Datum floatDatum; floatDatum.set_type(DatumType::Float);
				Assert::ExpectException<std::invalid_argument>([&]() { floatDatum.set_external_int_storage(newMemory, newMemorySize); });
			}
			{
				Datum myDatum;
				float myData = 1_f;
				float myData2 = 2_f;
				float newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_float_storage(newMemory, newMemorySize);
				myDatum.set_float_at(1, myData2);
				Assert::AreEqual(myDatum.get_float_at(0), myData);
				Assert::AreEqual(myDatum.get_float_at(1), myData2);
				Assert::AreEqual(myDatum.get_float_at(2), myData);
				Assert::AreEqual(newMemory[1], myData2);

				Datum myDatum2;
				float myData3 = 3_f;
				myDatum2.set_external_void_storage(newMemory, newMemorySize, DatumType::Float);
				myDatum2.set_float_at(1, myData3);
				Assert::AreEqual(myDatum2.get_float_at(0), myData);
				Assert::AreEqual(myDatum2.get_float_at(1), myData3);
				Assert::AreEqual(myDatum2.get_float_at(2), myData);
				Assert::AreEqual(newMemory[1], myData3);

				Datum intDatum; intDatum.set_type(DatumType::Integer);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_float_storage(newMemory, newMemorySize); });
			}
			{
				Datum myDatum;
				glm::vec4 myData(0, 1, 2, 3);
				glm::vec4 myData2(10, 11, 12, 13);
				glm::vec4 newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_vec4_storage(newMemory, newMemorySize);
				myDatum.set_vec4_at(1, myData2);
				Assert::AreEqual(myDatum.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum.get_vec4_at(1), myData2);
				Assert::AreEqual(myDatum.get_vec4_at(2), myData);
				Assert::AreEqual(newMemory[1], myData2);

				Datum myDatum2;
				glm::vec4 myData3(20, 21, 22, 23);
				myDatum2.set_external_void_storage(newMemory, newMemorySize, DatumType::Vector4);
				myDatum2.set_vec4_at(1, myData3);
				Assert::AreEqual(myDatum2.get_vec4_at(0), myData);
				Assert::AreEqual(myDatum2.get_vec4_at(1), myData3);
				Assert::AreEqual(myDatum2.get_vec4_at(2), myData);
				Assert::AreEqual(newMemory[1], myData3);

				Datum intDatum; intDatum.set_type(DatumType::Integer);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_vec4_storage(newMemory, newMemorySize); });
			}
			{
				Datum myDatum;
				glm::mat4 myData = glm::mat4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
				glm::mat4 myData2 = glm::mat4(10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25);
				glm::mat4 newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_mat4_storage(newMemory, newMemorySize);
				myDatum.set_mat4_at(1, myData2);
				Assert::AreEqual(myDatum.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum.get_mat4_at(1), myData2);
				Assert::AreEqual(myDatum.get_mat4_at(2), myData);
				Assert::AreEqual(newMemory[1], myData2);

				Datum myDatum2;
				glm::mat4 myData3 = glm::mat4(100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250);
				myDatum2.set_external_void_storage(newMemory, newMemorySize, DatumType::Matrix4);
				myDatum2.set_mat4_at(1, myData3);
				Assert::AreEqual(myDatum2.get_mat4_at(0), myData);
				Assert::AreEqual(myDatum2.get_mat4_at(1), myData3);
				Assert::AreEqual(myDatum2.get_mat4_at(2), myData);
				Assert::AreEqual(newMemory[1], myData3);

				Datum intDatum; intDatum.set_type(DatumType::Integer);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_mat4_storage(newMemory, newMemorySize); });
			}
			{
				Datum myDatum;
				std::string myData = "mogu mogu";
				std::string myData2 = "okayu";
				std::string newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_string_storage(newMemory, newMemorySize);
				myDatum.set_string_at(1, myData2);
				Assert::AreEqual(myDatum.get_string_at(0), myData);
				Assert::AreEqual(myDatum.get_string_at(1), myData2);
				Assert::AreEqual(myDatum.get_string_at(2), myData);
				Assert::AreEqual(newMemory[1], myData2);

				Datum myDatum2;
				std::string myData3 = "kon kon kitsune";
				myDatum2.set_external_void_storage(newMemory, newMemorySize, DatumType::String);
				myDatum2.set_string_at(1, myData3);
				Assert::AreEqual(myDatum2.get_string_at(0), myData);
				Assert::AreEqual(myDatum2.get_string_at(1), myData3);
				Assert::AreEqual(myDatum2.get_string_at(2), myData);
				Assert::AreEqual(newMemory[1], myData3);

				Datum intDatum; intDatum.set_type(DatumType::Integer);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_string_storage(newMemory, newMemorySize); });
			}
		}

		TEST_METHOD(VariousExceptions)
		{
			{
				Datum intDatum; intDatum.push_int_back(1); intDatum.clear();
				Datum extDatum; int ext[5]{ 0, 1, 2, 3, 4 }; extDatum.set_external_int_storage(ext, 5);
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum = extDatum; });
			}
			{
				Datum intDatum; intDatum.push_int_back(1); intDatum.clear();
				Datum extDatum; int ext[5]{ 0, 1, 2, 3, 4 }; extDatum.set_external_int_storage(ext, 5);
				//Assert::AreEqual(intDatum = std::move(extDatum), intDatum);
			}
			{
				Datum intDatum; intDatum.push_int_back(1); intDatum.clear();
				Datum floatDatum; floatDatum.push_float_back(1); floatDatum.clear();
				Datum vecDatum; vecDatum.push_vec4_back(glm::vec4()); vecDatum.clear();
				Datum matDatum; matDatum.push_mat4_back(glm::mat4()); matDatum.clear();
				Datum strDatum; strDatum.push_string_back(std::string()); strDatum.clear();
				int ei[5];
				float ef[5];
				glm::vec4 evec[5];
				glm::mat4 emat[5];
				std::string estr[5];
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_int_storage(ei, 5); });
				Assert::ExpectException<std::invalid_argument>([&]() { intDatum.set_external_float_storage(ef, 5); });
				Assert::ExpectException<std::invalid_argument>([&]() { floatDatum.set_external_float_storage(ef, 5); });
				Assert::ExpectException<std::invalid_argument>([&]() { vecDatum.set_external_vec4_storage(evec, 5); });
				Assert::ExpectException<std::invalid_argument>([&]() { matDatum.set_external_mat4_storage(emat, 5); });
				Assert::ExpectException<std::invalid_argument>([&]() { strDatum.set_external_string_storage(estr, 5); });
			}
		}
		
		TEST_METHOD(PredicateTest)
		{
			{
				Datum myDatum;
				Assert::IsTrue(myDatum.is_unknown());
				myDatum.set_type(DatumType::Integer);
				Assert::IsFalse(myDatum.is_unknown());
			}
			{
				Datum myDatum;
				Assert::IsFalse(myDatum.is_compatible_with(DatumType::Integer));
				myDatum.set_type(DatumType::Integer);
				Assert::IsTrue(myDatum.is_compatible_with(DatumType::Integer));
			}
			{
				Datum myDatum;
				Assert::IsFalse(myDatum.is_external());
				int myData = 1;
				int newMemory[5]{ myData, myData, myData, myData, myData };
				size_t newMemorySize = 5_z;
				myDatum.set_external_int_storage(newMemory, newMemorySize);
				Assert::IsTrue(myDatum.is_external());

				Datum myDatum2(myDatum);
				Assert::IsTrue(myDatum2.is_external());
			}
		}

		TEST_METHOD(ScopeTesting)
		{
			// Datum(const ScopePtr&);
			{
				Scope scope(1);
				ScopePtr myData = &scope;
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
			}
			// const ScopePtr& get_scope_at(size_t) const;
			// ScopePtr& get_scope_at(size_t);
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum.push_scope_back(myData);
				myDatum.push_scope_back(myData2);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreEqual(myDatum.get_scope_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.get_scope_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_int_at(0); });
				Assert::AreEqual(myConstDatum.get_scope_at(0), (ConstScopePtr)myData);
				Assert::AreEqual(myConstDatum.get_scope_at(1), (ConstScopePtr)myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_scope_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
			}
			// Scope& operator[](size_t);
			// const Scope& operator[](size_t) const;
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum.push_scope_back(myData);
				myDatum.push_scope_back(myData2);
				Assert::AreEqual(myDatum[0], scope);
				Assert::AreEqual(myDatum[1], scope2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum[3]; });
				Assert::AreEqual(myConstDatum[0], scope);
				Assert::AreEqual(myConstDatum[1], scope2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum[3]; });
			}
			// Datum& operator=(const ScopePtr&);
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				myDatum.push_scope_back(myData); myDatum.push_scope_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(); });
			}
			// ScopePtr& set_scope_at(size_t, const ScopePtr&);
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum.push_scope_back(myData); myDatum.push_scope_back(myData); myDatum.push_scope_back(myData);
				myDatum.set_scope_at(1, myData2);
				ScopePtr& temp = myDatum.set_scope_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreEqual(myDatum.get_scope_at(1), myData2);
				Assert::AreEqual(myDatum.get_scope_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_scope_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
			}
			// void push_scope_front(const ScopePtr&);
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum.push_scope_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreNotSame(myDatum.get_scope_at(0), myData);
				myDatum.push_scope_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData2);
				Assert::AreEqual(myDatum.get_scope_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
			}
			// void push_scope_back(const ScopePtr&);
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				myDatum.push_scope_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreNotSame(myDatum.get_scope_at(0), myData);
				myDatum.push_scope_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreEqual(myDatum.get_scope_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
			}
			// bool find_scope(const ConstScopePtr&, size_t*) const;
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				Scope scope3(3);
				Scope scope4(4);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				ScopePtr myData3 = &scope3;
				ScopePtr myData4 = &scope4;
				myDatum.push_scope_back(myData); myDatum.push_scope_back(myData2); myDatum.push_scope_back(myData3);
				size_t indexFoundAt = 0;
				Assert::IsTrue(myDatum.find_scope(myData, &indexFoundAt));
				Assert::AreEqual(indexFoundAt, 0_z);
				Assert::IsTrue(myDatum.find_scope(myData3, &indexFoundAt));
				Assert::AreEqual(indexFoundAt, 2_z);
				Assert::IsTrue(myDatum.find_scope(myData2, &indexFoundAt));
				Assert::AreEqual(indexFoundAt, 1_z);
				Assert::IsFalse(myDatum.find_scope(myData4, &indexFoundAt));
			}
			// void remove_scope_at(size_t);
			{
				Datum myDatum;
				Scope scope(1);
				Scope scope2(2);
				Scope scope3(3);
				ScopePtr myData = &scope;
				ScopePtr myData2 = &scope2;
				ScopePtr myData3 = &scope3;
				myDatum.push_scope_back(myData); myDatum.push_scope_back(myData2); myDatum.push_scope_back(myData3);
				myDatum.remove_scope_at(1);
				Assert::AreEqual(myDatum.get_type(), DatumType::Table);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData);
				Assert::AreEqual(myDatum.get_scope_at(1), myData3);
				myDatum.remove_scope_at(0);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_scope_at(0), myData3);
				myDatum.remove_scope_at(0);
				Assert::AreEqual(myDatum.size(), 0_z);
			}
		}

		TEST_METHOD(RTTITesting)
		{
			// Datum(const RTTIPtr&);
			{
				Foo foo(1);
				RTTIPtr myData = &foo;
				Datum myDatum(myData);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Pointer);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
			}
			// const RTTIPtr& get_RTTI_at(size_t) const;
			// RTTIPtr& get_RTTI_at(size_t);
			{
				Datum myDatum;
				const Datum& myConstDatum = myDatum;
				Foo foo(1);
				Foo foo2(2);
				RTTIPtr myData = &foo;
				RTTIPtr myData2 = &foo2;
				myDatum.push_RTTI_back(myData);
				myDatum.push_RTTI_back(myData2);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				Assert::AreEqual(myDatum.get_RTTI_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myDatum]() { myDatum.get_RTTI_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.get_int_at(0); });
				Assert::AreEqual(myConstDatum.get_RTTI_at(0), myData);
				Assert::AreEqual(myConstDatum.get_RTTI_at(1), myData2);
				Assert::ExpectException<std::out_of_range>([&myConstDatum]() { myConstDatum.get_RTTI_at(3); });
				Assert::ExpectException<std::invalid_argument>([&myConstDatum]() { myConstDatum.get_int_at(0); });
			}
			// Datum& operator=(const RTTIPtr&);
			{
				Datum myDatum;
				Foo foo(1);
				Foo foo2(2);
				RTTIPtr myData = &foo;
				RTTIPtr myData2 = &foo2;
				myDatum = myData;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Pointer);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				myDatum.push_RTTI_back(myData); myDatum.push_RTTI_back(myData);
				Assert::AreEqual(myDatum.size(), 3_z);
				myDatum = myData2;
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum = int(); });
			}
			// RTTIPtr& set_RTTI_at(size_t, const RTTIPtr&);
			{
				Datum myDatum;
				Foo foo(1);
				Foo foo2(2);
				RTTIPtr myData = &foo;
				RTTIPtr myData2 = &foo2;
				myDatum.push_RTTI_back(myData); myDatum.push_RTTI_back(myData); myDatum.push_RTTI_back(myData);
				myDatum.set_RTTI_at(1, myData2);
				RTTIPtr& temp = myDatum.set_RTTI_at(2, myData2); temp = myData;
				Assert::AreEqual(myDatum.size(), 3_z);
				Assert::AreEqual(myDatum.get_type(), DatumType::Pointer);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				Assert::AreEqual(myDatum.get_RTTI_at(1), myData2);
				Assert::AreEqual(myDatum.get_RTTI_at(2), myData);
				Assert::ExpectException<std::out_of_range>([&]() { myDatum.set_RTTI_at(3, myData2); });
				Assert::ExpectException<std::invalid_argument>([&]() { myDatum.set_int_at(0, int()); });
			}
			// void push_RTTI_front(const RTTIPtr&);
			{
				Datum myDatum;
				Foo foo(1);
				Foo foo2(2);
				RTTIPtr myData = &foo;
				RTTIPtr myData2 = &foo2;
				myDatum.push_RTTI_front(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Pointer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				Assert::AreNotSame(myDatum.get_RTTI_at(0), myData);
				myDatum.push_RTTI_front(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData2);
				Assert::AreEqual(myDatum.get_RTTI_at(1), myData);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_front(0); });
			}
			// void push_RTTI_back(const RTTIPtr&);
			{
				Datum myDatum;
				Foo foo(1);
				Foo foo2(2);
				RTTIPtr myData = &foo;
				RTTIPtr myData2 = &foo2;
				myDatum.push_RTTI_back(myData);
				Assert::AreEqual(myDatum.get_type(), DatumType::Pointer);
				Assert::AreEqual(myDatum.size(), 1_z);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				Assert::AreNotSame(myDatum.get_RTTI_at(0), myData);
				myDatum.push_RTTI_back(myData2);
				Assert::AreEqual(myDatum.size(), 2_z);
				Assert::AreEqual(myDatum.get_RTTI_at(0), myData);
				Assert::AreEqual(myDatum.get_RTTI_at(1), myData2);
				Assert::ExpectException<std::invalid_argument>([&myDatum]() { myDatum.push_int_back(0); });
			}
		}

	private:
		// ported from FooTests.cpp
		inline static _CrtMemState _startMemState;
	};
}
