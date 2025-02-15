#include "pch.h"
#include <iostream>
#include "CppUnitTest.h"
#include <vector>

#include "Datum.h"
#include "Scope.h"

#include "TestDefinitions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
	TEST_CLASS(ScopeTest)
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
			Scope myScope;
			Assert::AreEqual(myScope.size(), 0_z);
			Scope myScope2;
			Assert::AreEqual(myScope, myScope2);
			Assert::AreNotSame(myScope, myScope2);
		}

		TEST_METHOD(CopyConstructor)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			{
				// We're going to create some very cursed scopes
				Scope* rootScope1 = new Scope();
				int myData1 = 1;
				int myData2 = 2;
				int myData3 = 3;
				std::string rootKey1 = "ron";
				std::string rootKey2 = "nya";

				Scope* myScope = new Scope();
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope->add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* rootScope1Datum1 = rootScope1->add_datum(rootKey1);
				rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
				rootScope1->adopt_scope(myScope, rootKey2);
				{
					Scope rootScope2(*rootScope1);
					Assert::AreEqual(*rootScope1, rootScope2);
					Assert::AreNotSame(*rootScope1, rootScope2);
					Assert::AreEqual((*rootScope1)[0], rootScope2[0]);
					Assert::AreNotSame((*rootScope1)[0], rootScope2[0]);
				}
				delete myScope;
				delete rootScope1;
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			{
				// We're going to create some very cursed scopes
				Scope* rootScope1 = new Scope();
				int myData1 = 1;
				int myData2 = 2;
				int myData3 = 3;
				std::string rootKey1 = "ron";
				std::string rootKey2 = "nya";

				Scope* myScope = new Scope();
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope->add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* rootScope1Datum1 = rootScope1->add_datum(rootKey1);
				rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
				rootScope1->adopt_scope(myScope, rootKey2);
				
				Scope rootScope2(std::move(*rootScope1));
				delete myScope;
				delete rootScope1;
			}
		}

		TEST_METHOD(AssignmentOperator)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
			"mogu mogu": childScope
			"nekomata":  grandchildScope1
			"grr grr":  grandgrandchildScope1
			"tummy":	grandgrandchildScope2
			grandgrandchildScope3
			grandchildScope2
			"okayu":	 grandchildScope3
			grandchildScope4
			*/
			{
				// We're going to create some very cursed scopes
				Scope rootScope1;
				int myData1 = 1;
				int myData2 = 2;
				int myData3 = 3;
				std::string rootKey1 = "ron";
				std::string rootKey2 = "nya";

				Scope myScope;
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope.add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* rootScope1Datum1 = rootScope1.add_datum(rootKey1);
				rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
				rootScope1.adopt_scope(&myScope, rootKey2);
				{
					Scope rootScope2;
					Assert::AreNotEqual(rootScope1, rootScope2);

					rootScope2 = rootScope1;
					Assert::AreEqual(rootScope1, rootScope2);
					Assert::AreNotSame(rootScope1, rootScope2);
					Assert::AreEqual(rootScope1[0], rootScope2[0]);
					Assert::AreNotSame(rootScope1[0], rootScope2[0]);
				}
				{
					Scope rootScope2;
					rootScope2.add_empty_scope(rootKey2)->add_empty_scope(rootKey2);
					rootScope2 = rootScope1;
					Assert::AreEqual(rootScope1, rootScope2);
					Assert::AreNotSame(rootScope1, rootScope2);
					Assert::AreEqual(rootScope1[0], rootScope2[0]);
					Assert::AreNotSame(rootScope1[0], rootScope2[0]);
				}
			}
		}

		TEST_METHOD(MoveOperator)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			{
				// We're going to create some very cursed scopes
				Scope* rootScope1 = new Scope();
				int myData1 = 1;
				int myData2 = 2;
				int myData3 = 3;
				std::string rootKey1 = "ron";
				std::string rootKey2 = "nya";

				Scope* myScope = new Scope();
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope->add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* rootScope1Datum1 = rootScope1->add_datum(rootKey1);
				rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
				rootScope1->adopt_scope(myScope, rootKey2);
				
				Scope rootScope2;
				rootScope2 = std::move(*rootScope1);
				delete myScope;
				delete rootScope1;
			}
		}

		TEST_METHOD(AddDatum)
		{
			Scope myScope;
			std::string key = "mogu";
			std::string key2 = "okayu";

			Datum* myDatumPtr1 = myScope.add_datum(key);
			Assert::AreEqual(myScope.size(), 1_z);
			Assert::AreEqual(myScope.get_datum_by_key(key), myDatumPtr1);

			Datum* myDatumPtr2 = myScope.add_datum(key2);
			Assert::AreEqual(myScope.size(), 2_z);
			Assert::AreEqual(myScope.get_datum_by_key(key), myDatumPtr1);
			Assert::AreEqual(myScope.get_datum_by_key(key2), myDatumPtr2);

			Datum* myDatumPtr3 = myScope.add_datum(key2);
			Assert::AreEqual(myScope.size(), 2_z);
			Assert::AreEqual(myScope.get_datum_by_key(key), myDatumPtr1);
			Assert::AreEqual(myScope.get_datum_by_key(key2), myDatumPtr2);
			Assert::AreEqual(myDatumPtr3, myDatumPtr2);
		}

		TEST_METHOD(GetParentScope)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			Scope myScope;
			std::string key = "mogu mogu";
			std::string childKey = "nekomata";
			std::string childKey2 = "okayu";
			std::string grandchildKey1 = "grr grr";
			std::string grandchildKey2 = "tummy";

			Scope* childScopePtr = myScope.add_empty_scope(key);
			Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
			Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
			Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

			const Scope& constmyScope = myScope;
			const Scope& constchildScope = *childScopePtr;
			const Scope& constgrandchildScope1 = *grandchildScopePtr1;
			const Scope& constgrandchildScope2 = *grandchildScopePtr2;
			const Scope& constgrandchildScope3 = *grandchildScopePtr3;
			const Scope& constgrandchildScope4 = *grandchildScopePtr4;
			const Scope& constgrandgrandchildScope1 = *grandgrandchildScopePtr1;
			const Scope& constgrandgrandchildScope2 = *grandgrandchildScopePtr2;
			const Scope& constgrandgrandchildScope3 = *grandgrandchildScopePtr3;

			Assert::AreEqual(constmyScope.parent_scope(), (Scope*)nullptr);
			Assert::AreEqual(constchildScope.parent_scope(), (Scope*)&myScope);
			Assert::AreEqual(constgrandchildScope1.parent_scope(), (Scope*)childScopePtr);
			Assert::AreEqual(constgrandchildScope2.parent_scope(), (Scope*)childScopePtr);
			Assert::AreEqual(constgrandchildScope3.parent_scope(), (Scope*)childScopePtr);
			Assert::AreEqual(constgrandchildScope4.parent_scope(), (Scope*)childScopePtr);
			Assert::AreEqual(constgrandgrandchildScope1.parent_scope(), (Scope*)grandchildScopePtr1);
			Assert::AreEqual(constgrandgrandchildScope2.parent_scope(), (Scope*)grandchildScopePtr1);
			Assert::AreEqual(constgrandgrandchildScope3.parent_scope(), (Scope*)grandchildScopePtr1);
		}

		TEST_METHOD(GetParentDatumAndIndex)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			Scope myScope;
			std::string key = "mogu mogu";
			std::string childKey = "nekomata";
			std::string childKey2 = "okayu";
			std::string grandchildKey1 = "grr grr";
			std::string grandchildKey2 = "tummy";
			Scope* childScopePtr = myScope.add_empty_scope(key);
			Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
			Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
			Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

			size_t indexFoundAt = 0;
			Datum* keyDatumPtr = &(myScope[0]);
			Datum* childKeyDatumPtr = &(*childScopePtr)[0];
			Datum* childKey2DatumPtr = &(*childScopePtr)[1];
			Datum* grandchildKey1DatumPtr = &(*grandchildScopePtr1)[0];
			Datum* grandchildKey2DatumPtr = &(*grandchildScopePtr1)[1];

			Assert::AreEqual(childScopePtr->parent_datum_and_index(&indexFoundAt), keyDatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(grandchildScopePtr1->parent_datum_and_index(&indexFoundAt), childKeyDatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(grandchildScopePtr2->parent_datum_and_index(&indexFoundAt), childKeyDatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);
			Assert::AreEqual(grandchildScopePtr3->parent_datum_and_index(&indexFoundAt), childKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(grandchildScopePtr4->parent_datum_and_index(&indexFoundAt), childKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);
			Assert::AreEqual(grandgrandchildScopePtr1->parent_datum_and_index(&indexFoundAt), grandchildKey1DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(grandgrandchildScopePtr2->parent_datum_and_index(&indexFoundAt), grandchildKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(grandgrandchildScopePtr3->parent_datum_and_index(&indexFoundAt), grandchildKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);

			const Scope& constmyScope = myScope;
			const Scope& constchildScope = *childScopePtr;
			const Scope& constgrandchildScope1 = *grandchildScopePtr1;
			const Scope& constgrandchildScope2 = *grandchildScopePtr2;
			const Scope& constgrandchildScope3 = *grandchildScopePtr3;
			const Scope& constgrandchildScope4 = *grandchildScopePtr4;
			const Scope& constgrandgrandchildScope1 = *grandgrandchildScopePtr1;
			const Scope& constgrandgrandchildScope2 = *grandgrandchildScopePtr2;
			const Scope& constgrandgrandchildScope3 = *grandgrandchildScopePtr3;
			Assert::AreEqual(constchildScope.parent_datum_and_index(&indexFoundAt), (const Datum*)keyDatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(constgrandchildScope1.parent_datum_and_index(&indexFoundAt), (const Datum*)childKeyDatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(constgrandchildScope2.parent_datum_and_index(&indexFoundAt), (const Datum*)childKeyDatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);
			Assert::AreEqual(constgrandchildScope3.parent_datum_and_index(&indexFoundAt), (const Datum*)childKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(constgrandchildScope4.parent_datum_and_index(&indexFoundAt), (const Datum*)childKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);
			Assert::AreEqual(constgrandgrandchildScope1.parent_datum_and_index(&indexFoundAt), (const Datum*)grandchildKey1DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(constgrandgrandchildScope2.parent_datum_and_index(&indexFoundAt), (const Datum*)grandchildKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 0_z);
			Assert::AreEqual(constgrandgrandchildScope3.parent_datum_and_index(&indexFoundAt), (const Datum*)grandchildKey2DatumPtr);
			Assert::AreEqual(indexFoundAt, 1_z);
		}

		TEST_METHOD(AddScope)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			{
				Scope myScope;
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope.add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Assert::AreEqual(myScope.parent_scope(), (Scope*)nullptr);
				Assert::AreEqual(childScopePtr->parent_scope(), &myScope);
				Assert::AreEqual(grandchildScopePtr1->parent_scope(), childScopePtr);
				Assert::AreEqual(grandchildScopePtr2->parent_scope(), childScopePtr);
				Assert::AreEqual(grandchildScopePtr3->parent_scope(), childScopePtr);
				Assert::AreEqual(grandchildScopePtr4->parent_scope(), childScopePtr);
				Assert::AreEqual(grandgrandchildScopePtr1->parent_scope(), grandchildScopePtr1);
				Assert::AreEqual(grandgrandchildScopePtr2->parent_scope(), grandchildScopePtr1);
				Assert::AreEqual(grandgrandchildScopePtr3->parent_scope(), grandchildScopePtr1);
				// Confirm that there are no memory leaks when myScope is destructed
			}
		}

		TEST_METHOD(GetDatum)
		{
			Scope myScope;
			Scope myScope2;
			const Scope& myConstScope = myScope;
			const Scope& myConstScope2 = myScope2;
			std::string key = "mogu";
			std::string key2 = "okayu";

			Assert::AreEqual(myScope.get_datum_by_key(key), (Datum*)nullptr);
			Assert::AreEqual(myConstScope.get_datum_by_key(key), (const Datum*)nullptr);

			Datum* myDatumPtr1 = myScope.add_datum(key);
			Datum* myDatumPtr2 = myScope.add_datum(key2);
			Assert::AreEqual(myScope.size(), 2_z);
			Assert::AreEqual(myScope.get_datum_by_key(key), myDatumPtr1);
			Assert::AreEqual(myScope.get_datum_by_key(key2), myDatumPtr2);

			Datum* myDatumPtr3 = myScope2.add_datum(key);
			Datum* myDatumPtr4 = myScope2.add_datum(key2);
			Assert::AreEqual(myScope2.size(), 2_z);
			Assert::AreEqual(myScope2.get_datum_by_key(key), myDatumPtr3);
			Assert::AreEqual(myScope2.get_datum_by_key(key2), myDatumPtr4);

			Assert::AreNotEqual(myDatumPtr1, myDatumPtr3);
			Assert::AreNotEqual(myDatumPtr2, myDatumPtr4);

			Assert::AreEqual(myConstScope.size(), 2_z);
			Assert::AreEqual(myConstScope2.size(), 2_z);
			Assert::AreEqual(myConstScope.get_datum_by_key(key), (const Datum*)myDatumPtr1);
			Assert::AreEqual(myConstScope.get_datum_by_key(key2), (const Datum*)myDatumPtr2);
			Assert::AreEqual(myConstScope2.get_datum_by_key(key), (const Datum*)myDatumPtr3);
			Assert::AreEqual(myConstScope2.get_datum_by_key(key2), (const Datum*)myDatumPtr4);
		}

		TEST_METHOD(GetDatumInParent)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			{
				Scope myScope;
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				std::string unrelatedKey = "hello i am emu otori";

				Scope* childScopePtr = myScope.add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* foundDatum = nullptr;
				Scope* foundScope = nullptr;

				foundDatum = myScope.get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, &myScope[0]);
				Assert::AreEqual(foundScope, &myScope);
				foundDatum = myScope.get_datum_in_parent_by_key(unrelatedKey, &foundScope);
				Assert::AreEqual(foundDatum, (Datum*)nullptr);

				foundDatum = childScopePtr->get_datum_in_parent_by_key(childKey, &foundScope);
				Assert::AreEqual(foundDatum, &(*childScopePtr)[0]);
				Assert::AreEqual(foundScope, childScopePtr);
				foundDatum = childScopePtr->get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, &myScope[0]);
				Assert::AreEqual(foundScope, &myScope);

				foundDatum = grandchildScopePtr1->get_datum_in_parent_by_key(grandchildKey2, &foundScope);
				Assert::AreEqual(foundDatum, &(*grandchildScopePtr1)[1]);
				Assert::AreEqual(foundScope, grandchildScopePtr1);
				foundDatum = grandchildScopePtr1->get_datum_in_parent_by_key(key, nullptr);
				Assert::AreEqual(foundDatum, &myScope[0]);
				foundDatum = grandchildScopePtr1->get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, &myScope[0]);
				Assert::AreEqual(foundScope, &myScope);
				foundDatum = grandchildScopePtr1->get_datum_in_parent_by_key(unrelatedKey, nullptr);
				Assert::AreEqual(foundDatum, (Datum*)nullptr);
			}
			{
				Scope myScope;
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				std::string unrelatedKey = "hello i am emu otori";

				Scope* childScopePtr = myScope.add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				const Datum* foundDatum = nullptr;
				const Scope* foundScope = nullptr;

				const Scope* constmyScopePtr = &myScope;
				const Scope* constchildScopePtr = childScopePtr;
				const Scope* constgrandchildScopePtr1 = grandchildScopePtr1;
				const Datum* constkeyDatumPtr = &(myScope[0]);
				const Datum* constchildKeyDatumPtr = &(*childScopePtr)[0];
				const Datum* constgrandchildKey2DatumPtr = &(*grandchildScopePtr1)[1];

				foundDatum = constmyScopePtr->get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, constkeyDatumPtr);
				Assert::AreEqual(foundScope, constmyScopePtr);
				foundDatum = constmyScopePtr->get_datum_in_parent_by_key(unrelatedKey, &foundScope);
				Assert::AreEqual(foundDatum, (const Datum*)nullptr);

				foundDatum = constchildScopePtr->get_datum_in_parent_by_key(childKey, &foundScope);
				Assert::AreEqual(foundDatum, constchildKeyDatumPtr);
				Assert::AreEqual(foundScope, constchildScopePtr);
				foundDatum = constchildScopePtr->get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, constkeyDatumPtr);
				Assert::AreEqual(foundScope, constmyScopePtr);

				foundDatum = constgrandchildScopePtr1->get_datum_in_parent_by_key(grandchildKey2, &foundScope);
				Assert::AreEqual(foundDatum, constgrandchildKey2DatumPtr);
				Assert::AreEqual(foundScope, constgrandchildScopePtr1);
				foundDatum = constgrandchildScopePtr1->get_datum_in_parent_by_key(key, nullptr);
				Assert::AreEqual(foundDatum, constkeyDatumPtr);
				foundDatum = constgrandchildScopePtr1->get_datum_in_parent_by_key(key, &foundScope);
				Assert::AreEqual(foundDatum, constkeyDatumPtr);
				Assert::AreEqual(foundScope, constmyScopePtr);
				foundDatum = constgrandchildScopePtr1->get_datum_in_parent_by_key(unrelatedKey, nullptr);
				Assert::AreEqual(foundDatum, (const Datum*)nullptr);
			}
		}

		TEST_METHOD(AdoptScope)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
												"tummy":	grandgrandchildScope2
															grandgrandchildScope3
											 grandchildScope2
								"okayu":	 grandchildScope3
											 grandchildScope4
			*/
			Scope myScope;
			std::string key = "mogu mogu";
			std::string childKey = "nekomata";
			std::string childKey2 = "okayu";
			std::string grandchildKey1 = "grr grr";
			std::string grandchildKey2 = "tummy";

			Scope* myScopePtr = &myScope;
			Scope* childScopePtr = myScope.add_empty_scope(key);
			Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
			Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
			Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
			// Adopt three times so the tree looks like
			/*
			myScope (root scope)
				"mogu mogu": childScope
								"nekomata":  grandchildScope1
												"grr grr":  grandgrandchildScope1
															+grandchildScope3
												"tummy":	-(grandgrandchildScope2)
															-(grandgrandchildScope3)
											 grandchildScope2
								"okayu":	 -(grandchildScope3)
											 grandchildScope4
							  +grandgrandchildScope2
							  +grandgrandchildScope3
			*/
			/*
			grandchildScopePtr1->adopt_scope(grandchildScopePtr3, grandchildKey1);
			myScopePtr->adopt_scope(grandgrandchildScopePtr2, key);
			myScopePtr->adopt_scope(grandgrandchildScopePtr3, key);

			Assert::AreEqual(grandchildScopePtr3->parent_scope(), grandchildScopePtr1);
			Assert::AreEqual(grandgrandchildScopePtr2->parent_scope(), myScopePtr);
			Assert::AreEqual(grandgrandchildScopePtr2->parent_scope(), myScopePtr);

			Assert::IsTrue(grandchildScopePtr3->IsDescendantOf(grandchildScopePtr1));

			Assert::IsTrue(grandgrandchildScopePtr2->IsDescendantOf(myScopePtr));
			Assert::IsFalse(grandgrandchildScopePtr2->IsDescendantOf(grandchildScopePtr1));

			Assert::IsTrue(grandgrandchildScopePtr3->IsDescendantOf(myScopePtr));
			Assert::IsFalse(grandgrandchildScopePtr3->IsDescendantOf(grandchildScopePtr1));
			*/
		}

#pragma region AncestorAndDescendantTest
		//TEST_METHOD(AncestorAndDescendant)
		//{
		//	// This tree is gonna be nasty
		//	/*
		//	myScope (root scope)
		//		"mogu mogu": childScope
		//						"nekomata":  grandchildScope1
		//										"grr grr":  grandgrandchildScope1
		//										"tummy":	grandgrandchildScope2
		//													grandgrandchildScope3
		//									 grandchildScope2
		//						"okayu":	 grandchildScope3
		//									 grandchildScope4
		//	*/
		//	Scope myScope; Scope* myScopePtr = &myScope;
		//	std::string key = "mogu mogu";
		//	std::string childKey = "nekomata";
		//	std::string childKey2 = "okayu";
		//	std::string grandchildKey1 = "grr grr";
		//	std::string grandchildKey2 = "tummy";
		//	Scope* childScopePtr = myScope.add_scope(key);
		//	Scope* grandchildScopePtr1 = childScopePtr->add_scope(childKey);
		//	Scope* grandchildScopePtr2 = childScopePtr->add_scope(childKey);
		//	Scope* grandchildScopePtr3 = childScopePtr->add_scope(childKey2);
		//	Scope* grandchildScopePtr4 = childScopePtr->add_scope(childKey2);
		//	Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_scope(grandchildKey1);
		//	Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_scope(grandchildKey2);
		//	Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_scope(grandchildKey2);

		//	Assert::IsTrue(myScopePtr->IsAncestorOf(childScopePtr));
		//	Assert::IsTrue(childScopePtr->IsDescendantOf(myScopePtr));
		//	Assert::IsFalse(myScopePtr->IsDescendantOf(childScopePtr));
		//	Assert::IsFalse(childScopePtr->IsAncestorOf(myScopePtr));

		//	Assert::IsTrue(myScopePtr->IsAncestorOf(grandgrandchildScopePtr3));
		//	Assert::IsTrue(grandgrandchildScopePtr3->IsDescendantOf(myScopePtr));
		//	Assert::IsFalse(myScopePtr->IsDescendantOf(grandgrandchildScopePtr3));
		//	Assert::IsFalse(grandgrandchildScopePtr3->IsAncestorOf(myScopePtr));

		//	Assert::IsTrue(grandchildScopePtr1->IsAncestorOf(grandgrandchildScopePtr2));
		//	Assert::IsTrue(grandgrandchildScopePtr2->IsDescendantOf(grandchildScopePtr1));
		//	Assert::IsFalse(grandchildScopePtr1->IsDescendantOf(grandgrandchildScopePtr2));
		//	Assert::IsFalse(grandgrandchildScopePtr2->IsAncestorOf(grandchildScopePtr1));

		//	Assert::IsFalse(grandchildScopePtr3->IsAncestorOf(grandchildScopePtr4));
		//	Assert::IsFalse(grandchildScopePtr4->IsDescendantOf(grandchildScopePtr3));
		//	Assert::IsFalse(grandchildScopePtr4->IsAncestorOf(grandchildScopePtr3));
		//	Assert::IsFalse(grandchildScopePtr3->IsDescendantOf(grandchildScopePtr4));
		//}
#pragma endregion

		TEST_METHOD(OperatorSquareBracketsString)
		{
			Scope myScope;
			std::string key = "mogu";
			std::string key2 = "okayu";
			int myData = 1;
			int myData2 = 2;

			Assert::AreEqual(myScope.size(), 0_z);

			myScope[key] = myData;
			Assert::AreEqual(myScope.size(), 1_z);
			Assert::AreEqual(myScope[key], *myScope.get_datum_by_key(key));
			Assert::IsTrue(myScope[key] == myData);

			myScope[key2] = myData;
			Assert::AreEqual(myScope.size(), 2_z);
			Assert::AreEqual(myScope[key2], *myScope.get_datum_by_key(key2));
			Assert::IsTrue(myScope[key] == myData);
			Assert::IsTrue(myScope[key2] == myData);

			myScope[key2] = myData2;
			Assert::AreEqual(myScope.size(), 2_z);
			Assert::AreEqual(myScope[key2], *myScope.get_datum_by_key(key2));
			Assert::IsTrue(myScope[key] == myData);
			Assert::IsTrue(myScope[key2] == myData2);

			Assert::AreEqual(myScope[0], myScope[key]);
			Assert::AreEqual(myScope[1], myScope[key2]);
		}

		TEST_METHOD(OperatorSquareBracketsIndex)
		{
			Scope myScope;
			std::string key = "mogu";
			std::string key2 = "okayu";
			std::string key3 = "!!!";

			myScope.add_datum(key);
			myScope.add_datum(key2);
			Assert::AreEqual(myScope[0], myScope[key]);
			Assert::AreEqual(myScope[1], myScope[key2]);
			myScope.add_datum(key2);
			myScope.add_datum(key);
			Assert::AreEqual(myScope[0], myScope[key]);
			Assert::AreEqual(myScope[1], myScope[key2]);

			myScope[key3];
			Assert::AreEqual(myScope[0], myScope[key]);
			Assert::AreEqual(myScope[1], myScope[key2]);
			Assert::AreEqual(myScope[2], myScope[key3]);
			myScope[key2];
			myScope[key];
			Assert::AreEqual(myScope[0], myScope[key]);
			Assert::AreEqual(myScope[1], myScope[key2]);
			Assert::AreEqual(myScope[2], myScope[key3]);

			//exception
			Assert::ExpectException<std::out_of_range>([&]() { myScope[3]; });
		}

		TEST_METHOD(EqualityOperator)
		{
			// We're going to create some very cursed scopes
			Scope rootScope1;
			int myData1 = 1;
			int myData2 = 2;
			int myData3 = 3;
			std::string rootKey1 = "ron";
			std::string rootKey2 = "nya";

			Scope myScope;
			std::string key = "mogu mogu";
			std::string childKey = "nekomata";
			std::string childKey2 = "okayu";
			std::string grandchildKey1 = "grr grr";
			std::string grandchildKey2 = "tummy";
			Scope* childScopePtr = myScope.add_empty_scope(key);
			Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
			Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
			Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
			Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
			Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

			Datum* rootScope1Datum1 = rootScope1.add_datum(rootKey1);
			rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
			rootScope1.adopt_scope(&myScope, rootKey2);
			// At this point, rootScope1 has two Datums. One datum has a size of three and has three ints. The other datum has a size of 1 with a Scope tree
			{
				Scope rootScope2;
				Assert::IsFalse(rootScope1 == rootScope2);
				Assert::IsTrue(rootScope1 != rootScope2);
			}
			{
				Scope rootScope2;
				Datum* rootScope2Datum1 = rootScope2.add_datum(rootKey1);
				rootScope2Datum1->push_int_back(myData1); rootScope2Datum1->push_int_back(myData2); rootScope2Datum1->push_int_back(myData3);
				Assert::IsFalse(rootScope1 == rootScope2);
				Assert::IsTrue(rootScope1 != rootScope2);
			}
			{
				Scope rootScope2;
				Datum* rootScope2Datum1 = rootScope2.add_datum(rootKey1);
				rootScope2Datum1->push_int_back(myData1); rootScope2Datum1->push_int_back(myData2); rootScope2Datum1->push_int_back(myData3);
				Scope _myScope;
				Scope* _childScopePtr = _myScope.add_empty_scope(key);
				Scope* _grandchildScopePtr1 = _childScopePtr->add_empty_scope(childKey);
				Scope* _grandchildScopePtr2 = _childScopePtr->add_empty_scope(childKey);
				Scope* _grandchildScopePtr3 = _childScopePtr->add_empty_scope(childKey2);
				Scope* _grandchildScopePtr4 = _childScopePtr->add_empty_scope(childKey2);
				Scope* _grandgrandchildScopePtr1 = _grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* _grandgrandchildScopePtr2 = _grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* _grandgrandchildScopePtr3 = _grandchildScopePtr1->add_empty_scope(grandchildKey2);
				rootScope2.adopt_scope(&_myScope, rootKey2);
				Assert::IsTrue(rootScope1 == rootScope2);
				Assert::IsFalse(rootScope1 != rootScope2);
			}
			{
				Scope rootScope2;
				Datum* rootScope2Datum1 = rootScope2.add_datum(rootKey1);
				rootScope2Datum1->push_int_back(myData1); rootScope2Datum1->push_int_back(myData2); rootScope2Datum1->push_int_back(myData3);
				Scope _myScope;
				Scope* _childScopePtr = _myScope.add_empty_scope(key);
				Scope* _grandchildScopePtr1 = _childScopePtr->add_empty_scope(childKey);
				Scope* _grandchildScopePtr2 = _childScopePtr->add_empty_scope(childKey);
				Scope* _grandchildScopePtr3 = _childScopePtr->add_empty_scope(childKey2);
				Scope* _grandchildScopePtr4 = _childScopePtr->add_empty_scope(childKey2);
				Scope* _grandgrandchildScopePtr1 = _grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* _grandgrandchildScopePtr2 = _grandchildScopePtr1->add_empty_scope(grandchildKey2);
				// missing one child
				rootScope2.adopt_scope(&_myScope, rootKey2);
				Assert::IsFalse(rootScope1 == rootScope2);
				Assert::IsTrue(rootScope1 != rootScope2);
			}
			{
				Scope rootScope2;
				Datum* rootScope2Datum1 = rootScope2.add_datum(rootKey1);
				rootScope2Datum1->push_int_back(myData1); rootScope2Datum1->push_int_back(myData2); rootScope2Datum1->push_int_back(myData3);
				std::string _key = "never";
				std::string _childKey = "gonna";
				std::string _childKey2 = "give";
				std::string _grandchildKey1 = "you";
				std::string _grandchildKey2 = "up";
				Scope _myScope;
				Scope* _childScopePtr = _myScope.add_empty_scope(_key);
				Scope* _grandchildScopePtr1 = _childScopePtr->add_empty_scope(_childKey);
				Scope* _grandchildScopePtr2 = _childScopePtr->add_empty_scope(_childKey);
				Scope* _grandchildScopePtr3 = _childScopePtr->add_empty_scope(_childKey2);
				Scope* _grandchildScopePtr4 = _childScopePtr->add_empty_scope(_childKey2);
				Scope* _grandgrandchildScopePtr1 = _grandchildScopePtr1->add_empty_scope(_grandchildKey1);
				Scope* _grandgrandchildScopePtr2 = _grandchildScopePtr1->add_empty_scope(_grandchildKey2);
				Scope* _grandgrandchildScopePtr3 = _grandchildScopePtr1->add_empty_scope(_grandchildKey2);
				rootScope2.adopt_scope(&_myScope, rootKey2);
				Assert::IsFalse(rootScope1 == rootScope2);
				Assert::IsTrue(rootScope1 != rootScope2);
			}
		}

		TEST_METHOD(CloneTest)
		{
			// This tree is gonna be nasty
			/*
			myScope (root scope)
			"mogu mogu": childScope
			"nekomata":  grandchildScope1
			"grr grr":  grandgrandchildScope1
			"tummy":	grandgrandchildScope2
			grandgrandchildScope3
			grandchildScope2
			"okayu":	 grandchildScope3
			grandchildScope4
			*/
			{
				// We're going to create some very cursed scopes
				Scope* rootScope1 = new Scope();
				int myData1 = 1;
				int myData2 = 2;
				int myData3 = 3;
				std::string rootKey1 = "ron";
				std::string rootKey2 = "nya";

				Scope* myScope = new Scope();
				std::string key = "mogu mogu";
				std::string childKey = "nekomata";
				std::string childKey2 = "okayu";
				std::string grandchildKey1 = "grr grr";
				std::string grandchildKey2 = "tummy";
				Scope* childScopePtr = myScope->add_empty_scope(key);
				Scope* grandchildScopePtr1 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr2 = childScopePtr->add_empty_scope(childKey);
				Scope* grandchildScopePtr3 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandchildScopePtr4 = childScopePtr->add_empty_scope(childKey2);
				Scope* grandgrandchildScopePtr1 = grandchildScopePtr1->add_empty_scope(grandchildKey1);
				Scope* grandgrandchildScopePtr2 = grandchildScopePtr1->add_empty_scope(grandchildKey2);
				Scope* grandgrandchildScopePtr3 = grandchildScopePtr1->add_empty_scope(grandchildKey2);

				Datum* rootScope1Datum1 = rootScope1->add_datum(rootKey1);
				rootScope1Datum1->push_int_back(myData1); rootScope1Datum1->push_int_back(myData2); rootScope1Datum1->push_int_back(myData3);
				rootScope1->adopt_scope(myScope, rootKey2);
				{
					Scope* rootScope2Ptr = rootScope1->Clone();
					Assert::AreEqual(*rootScope1, *rootScope2Ptr);
					Assert::AreNotSame(*rootScope1, *rootScope2Ptr);
					Assert::AreEqual((*rootScope1)[0], (*rootScope2Ptr)[0]);
					Assert::AreNotSame((*rootScope1)[0], (*rootScope2Ptr)[0]);
					delete rootScope2Ptr;
				}
				delete myScope;
				delete rootScope1;
			}
		}

	private:
		// ported from FooTests.cpp
		inline static _CrtMemState _startMemState;
	};
}
