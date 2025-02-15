#include "pch.h"
#include <iostream>
#include <string>
#include "CppUnitTest.h"
#include <filesystem>

#include "Definitions.h"
#include "TestDefinitions.h"
#include "TestClasses/TestParseHandler.h"
#include "TestClasses/IntHandler.h"
#include "ScopeHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
	TEST_CLASS(JSONTest)
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

        TEST_METHOD(JeremysUltimateTest) {            
            std::string nested = R"({
                "int":1,
                "obj":{
                    "int":1,
                    "obj":{
                        "int":1,
                        "str":"abc"
                    },
                    "str":"abc"
                },
                "obj2":{
                    "int":1,
                    "obj":{
                        "int":1,
                        "str":"abc"
                    },
                    "str":"abc"
                },
                "str":"abc"
            })"_s;
            TestWrapper* wrapper = new TestWrapper();
            TestParseHandler* testParseHandler = new TestParseHandler();
            ParseCoordinator parser(wrapper);
            parser.add_handler(testParseHandler);
            bool deserialized = parser.deserialize_json_string(nested);
            Assert::IsTrue(deserialized);
            Assert::IsTrue(testParseHandler->initializeCalled);
            Assert::IsTrue(testParseHandler->cleanupCalled);
            Assert::AreEqual(14_z, testParseHandler->startCount);
            Assert::AreEqual(14_z, testParseHandler->endCount);
            Assert::AreEqual(3_z, wrapper->maxDepth);

            delete testParseHandler;
            delete wrapper;
        }

        TEST_METHOD(BasicTest)
        {
            // succeed in reading a json
            {
                /*
                std::string jsonString = R"({
                    "int":1,
                    "str":"mogu mogu okayu"
                })"_s;
                */
                std::string jsonString = R"({
                    "int":1,
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"_s;
                TestWrapper* myWrapper = new TestWrapper();
                TestParseHandler* myHandler = new TestParseHandler();

                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);
                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                delete myHandler;
                delete myWrapper;
            }
            // fail to read a json
            {
                std::string invalidString = R"({
                    "int":1
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"_s;
                TestWrapper* myWrapper = new TestWrapper();
                TestParseHandler* myHandler = new TestParseHandler();

                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);
                bool success = PC.deserialize_json_string(invalidString);

                Assert::IsFalse(success);
                delete myHandler;
                delete myWrapper;
            }
        }

        TEST_METHOD(IntWrapperTest)
        {
            // Single int with correct wrapper and one handler
            {
                std::string jsonString = R"({
                    "int" : 1
                })";
                IntWrapper* myWrapper = new IntWrapper();
                IntHandler* myHandler = new IntHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myWrapper->get_int(), 1);

                delete myHandler;
                delete myWrapper;
            }
            // int array with correct wrapper and one handler
            {
                std::string jsonString = R"({
                    "int" : [1, 2, 3]
                })";
                IntWrapper* myWrapper = new IntWrapper();
                IntHandler* myHandler = new IntHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                std::vector<int> expected({ 1, 2, 3 });
                Assert::IsTrue(myWrapper->get_int_array() == expected); // using IsTrue to get around static assert for Assert::AreEqual

                delete myHandler;
                delete myWrapper;
            }
            // Single int with correct wrapper and multiple handlers
            {
                std::string jsonString = R"({
                    "int" : 1
                })";
                IntWrapper* myWrapper = new IntWrapper();
                IntHandler* myHandler = new IntHandler();
                TestParseHandler* myHandler2 = new TestParseHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);
                PC.add_handler(myHandler2);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myWrapper->get_int(), 1);

                delete myHandler2;
                delete myHandler;
                delete myWrapper;
            }
            // Single int with incorrect wrapper and one handler
            {
                std::string jsonString = R"({
                    "int" : 1
                })";
                TestWrapper* myWrapper = new TestWrapper();
                IntHandler* myHandler = new IntHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsFalse(success);

                delete myHandler;
                delete myWrapper;
            }
            // int array with incorrect wrapper and multiple handlers (second can handle incorrect wrapper)
            {
                std::string jsonString = R"({
                    "int" : [1, 2, 3]
                })";
                TestWrapper* myWrapper = new TestWrapper();
                IntHandler* myHandler = new IntHandler();
                TestParseHandler* myHandler2 = new TestParseHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);
                PC.add_handler(myHandler2);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myWrapper->maxDepth, 1_z);

                delete myHandler2;
                delete myHandler;
                delete myWrapper;
            }
        }

        TEST_METHOD(ReadFromFileTest)
        {
            /*
            // test code
            {
                std::filesystem::path myDir = std::filesystem::current_path();

                std::string filename = "test.txt";
                std::ofstream fileoutstream(filename);
                bool success = !(fileoutstream.fail());
                fileoutstream << "test";
                fileoutstream.flush();
            }
            */
            
            // good file
            {
                std::string filename = R"(..\..\FieaGameEngine.test\TestFiles\testJsonString.txt)";
                TestWrapper* wrapper = new TestWrapper();
                TestParseHandler* testParseHandler = new TestParseHandler();
                ParseCoordinator parser(wrapper);
                parser.add_handler(testParseHandler);

                bool deserialized = parser.deserialize_json_file(filename);

                Assert::IsTrue(deserialized);
                Assert::IsTrue(testParseHandler->initializeCalled);
                Assert::IsTrue(testParseHandler->cleanupCalled);
                Assert::AreEqual(14_z, testParseHandler->startCount);
                Assert::AreEqual(14_z, testParseHandler->endCount);
                Assert::AreEqual(3_z, wrapper->maxDepth);

                delete testParseHandler;
                delete wrapper;
            }
            // bad file
            {
                std::string filename = R"(..\..\FieaGameEngine.test\TestFiles\testInvalidJsonString.txt)";
                TestWrapper* wrapper = new TestWrapper();
                TestParseHandler* testParseHandler = new TestParseHandler();
                ParseCoordinator parser(wrapper);
                parser.add_handler(testParseHandler);

                bool deserialized = parser.deserialize_json_file(filename);

                Assert::IsFalse(deserialized);

                delete testParseHandler;
                delete wrapper;
            }
        }

        TEST_METHOD(ReadFromStreamTest)
        {
            // good stream
            {
                std::stringstream ss(R"({
                    "int":1,
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"_s);
                std::istream stream(ss.rdbuf());
                TestWrapper* wrapper = new TestWrapper();
                TestParseHandler* testParseHandler = new TestParseHandler();
                ParseCoordinator parser(wrapper);
                parser.add_handler(testParseHandler);

                bool deserialized = parser.deserialize_json_stream(stream);

                Assert::IsTrue(deserialized);
                Assert::IsTrue(testParseHandler->initializeCalled);
                Assert::IsTrue(testParseHandler->cleanupCalled);
                Assert::AreEqual(14_z, testParseHandler->startCount);
                Assert::AreEqual(14_z, testParseHandler->endCount);
                Assert::AreEqual(3_z, wrapper->maxDepth);

                delete testParseHandler;
                delete wrapper;
            }
            // bad stream
            {
                std::stringstream ss(R"({
                    "int":1
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"_s);
                std::istream stream(ss.rdbuf());
                TestWrapper* wrapper = new TestWrapper();
                TestParseHandler* testParseHandler = new TestParseHandler();
                ParseCoordinator parser(wrapper);
                parser.add_handler(testParseHandler);

                bool deserialized = parser.deserialize_json_stream(stream);

                Assert::IsFalse(deserialized);

                delete testParseHandler;
                delete wrapper;
            }
        }

        TEST_METHOD(UnnestedScopeWrapperTest)
        {
            // Simple scope with one data type with one handler
            {
                std::string jsonString = R"({
                    "int" : 1
                })";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);
                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_int_at(0), 1);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Simple scope with all data types with one handler
            {
                std::string jsonString = R"-({
                    "int" : 1,
                    "float" : 2.5,
                    "vec4" : "vec4(1, 2, 3, 4)",
                    "mat4" : "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                    "string" : "mogu mogu okayu",
                    "int2" : 100
                })-";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 6_z);

                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_int_at(0), 1);
                Assert::AreEqual(myScope->get_datum_by_key("float"_s)->get_type(), DatumType::Float);
                Assert::AreEqual(myScope->get_datum_by_key("float"_s)->get_float_at(0), 2.5f);
                Assert::AreEqual(myScope->get_datum_by_key("vec4"_s)->get_type(), DatumType::Vector4);
                Assert::AreEqual(myScope->get_datum_by_key("vec4"_s)->get_vec4_at(0), glm::vec4(1, 2, 3, 4));
                Assert::AreEqual(myScope->get_datum_by_key("mat4"_s)->get_type(), DatumType::Matrix4);
                Assert::AreEqual(myScope->get_datum_by_key("mat4"_s)->get_mat4_at(0), glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
                Assert::AreEqual(myScope->get_datum_by_key("string"_s)->get_type(), DatumType::String);
                Assert::AreEqual(myScope->get_datum_by_key("string"_s)->get_string_at(0), "mogu mogu okayu"_s);
                Assert::AreEqual(myScope->get_datum_by_key("int2"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(myScope->get_datum_by_key("int2"_s)->get_int_at(0), 100);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Array scope with one data type with one handler
            {
                std::string jsonString = R"({
                    "int" : [1, 2, 3]
                })";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);
                Datum* myDatum = myScope->get_datum_by_key("int"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Integer);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_int_at(0), 1);
                Assert::AreEqual(myDatum->get_int_at(1), 2);
                Assert::AreEqual(myDatum->get_int_at(2), 3);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Array scope with all data types with one handler
            {
                std::string jsonString = R"-({
                    "int" : [1, 2, 3],
                    "float" : [2.5, 3.5, 4.5],
                    "vec4" : ["vec4(1, 2, 3, 4)", "vec4(10, 2, 3, 4)", "vec4(100, 2, 3, 4)"],
                    "mat4" : [
                        "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                        "mat4x4((10, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                        "mat4x4((100, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))"
                    ],
                    "string" : [
                        "mogu mogu okayu",
                        "yubi yubi",
                        "kon kon kitsune"
                    ],
                    "int2" : [100, 200, 300]
                })-";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 6_z);

                Datum* myDatum = myScope->get_datum_by_key("int"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Integer);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_int_at(0), 1);
                Assert::AreEqual(myDatum->get_int_at(1), 2);
                Assert::AreEqual(myDatum->get_int_at(2), 3);

                myDatum = myScope->get_datum_by_key("float"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Float);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_float_at(0), 2.5f);
                Assert::AreEqual(myDatum->get_float_at(1), 3.5f);
                Assert::AreEqual(myDatum->get_float_at(2), 4.5f);

                myDatum = myScope->get_datum_by_key("vec4"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Vector4);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_vec4_at(0), glm::vec4(1, 2, 3, 4));
                Assert::AreEqual(myDatum->get_vec4_at(1), glm::vec4(10, 2, 3, 4));
                Assert::AreEqual(myDatum->get_vec4_at(2), glm::vec4(100, 2, 3, 4));

                myDatum = myScope->get_datum_by_key("mat4"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Matrix4);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_mat4_at(0), glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
                Assert::AreEqual(myDatum->get_mat4_at(1), glm::mat4(10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
                Assert::AreEqual(myDatum->get_mat4_at(2), glm::mat4(100, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

                myDatum = myScope->get_datum_by_key("string"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::String);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_string_at(0), "mogu mogu okayu"_s);
                Assert::AreEqual(myDatum->get_string_at(1), "yubi yubi"_s);
                Assert::AreEqual(myDatum->get_string_at(2), "kon kon kitsune"_s);

                myDatum = myScope->get_datum_by_key("int2"_s);
                Assert::AreEqual(myDatum->get_type(), DatumType::Integer);
                Assert::AreEqual(myDatum->size(), 3_z);
                Assert::AreEqual(myDatum->get_int_at(0), 100);
                Assert::AreEqual(myDatum->get_int_at(1), 200);
                Assert::AreEqual(myDatum->get_int_at(2), 300);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

        TEST_METHOD(NestedScopeWrapperTest)
        {
            // Nested scope with one data type with one handler
            {
                std::string jsonString = R"({
                    "int" : 1,
                    "nestedScope" : {
                        "int" : 2,
                        "nestedNestedScope" : {
                            "int" : 3
                        }
                    }
                })";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 2_z);

                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_int_at(0), 1);
                Scope* nestedScope = myScope->get_datum_by_key("nestedScope")->get_scope_at(0);
                Assert::AreEqual(nestedScope->get_datum_by_key("int"_s)->get_int_at(0), 2);
                Scope* nestedNestedScope = nestedScope->get_datum_by_key("nestedNestedScope")->get_scope_at(0);
                Assert::AreEqual(nestedNestedScope->get_datum_by_key("int"_s)->get_int_at(0), 3);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Jeremy-provided scope tree
            {
                std::string jsonString = R"({
                    "int":1,
                    "obj":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "obj2":{
                        "int":1,
                        "obj":{
                            "int":1,
                            "str":"abc"
                        },
                        "str":"abc"
                    },
                    "str":"abc"
                })"_s;
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 4_z);

                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_int_at(0), 1);
                {
                    Scope* nestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                    Assert::AreEqual(nestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                    {
                        Scope* nestedNestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                    }
                    Assert::AreEqual(nestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                }
                {
                    Scope* nestedScope = myScope->get_datum_by_key("obj2"_s)->get_scope_at(0);
                    Assert::AreEqual(nestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                    {
                        Scope* nestedNestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                    }
                    Assert::AreEqual(nestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                }
                Assert::AreEqual(myScope->get_datum_by_key("str"_s)->get_string_at(0), "abc"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // From file
            {
                std::string filename = R"(..\..\FieaGameEngine.test\TestFiles\testJsonString.txt)";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_file(filename);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 4_z);

                Assert::AreEqual(myScope->get_datum_by_key("int"_s)->get_int_at(0), 1);
                {
                    Scope* nestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                    Assert::AreEqual(nestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                    {
                        Scope* nestedNestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                    }
                    Assert::AreEqual(nestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                }
                {
                    Scope* nestedScope = myScope->get_datum_by_key("obj2"_s)->get_scope_at(0);
                    Assert::AreEqual(nestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                    {
                        Scope* nestedNestedScope = myScope->get_datum_by_key("obj"_s)->get_scope_at(0);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("int")->get_int_at(0), 1);
                        Assert::AreEqual(nestedNestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                    }
                    Assert::AreEqual(nestedScope->get_datum_by_key("str")->get_string_at(0), "abc"_s);
                }
                Assert::AreEqual(myScope->get_datum_by_key("str"_s)->get_string_at(0), "abc"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

        TEST_METHOD(MiscMethodsTest)
        {
            // Single int with incorrect wrapper and one handler
            {
                std::string jsonString = R"({
                    "int" : 1
                })";
                TestWrapper* myWrapper = new TestWrapper();
                IntWrapper* myCorrectWrapper = new IntWrapper();
                IntHandler* myHandler = new IntHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                Assert::IsTrue(PC.get_wrapper() == myWrapper);
                PC.set_wrapper(myCorrectWrapper);
                Assert::IsFalse(PC.get_wrapper() == myWrapper);
                Assert::IsTrue(PC.get_wrapper() == myCorrectWrapper);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);

                delete myHandler;
                delete myCorrectWrapper;
                delete myWrapper;
            }
        }

	private:
		// ported from FooTests.cpp
		inline static _CrtMemState _startMemState;
	};
}
