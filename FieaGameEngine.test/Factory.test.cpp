#include "pch.h"
#include <iostream>
#include <string>
#include "CppUnitTest.h"
#include <filesystem>

#include "Definitions.h"
#include "TestDefinitions.h"

#include "FactoryManagerSingleton.hpp"
#include "ScopeHandler.h"

#include "GameObject.h"
#include "TestClasses/Monster.h"
#include "TestClasses/MonsterArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
    TEST_CLASS(FactoryTest)
    {
    public:
        // ported from FooTests.cpp
        TEST_METHOD_INITIALIZE(Initialize)
        {
#if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
#endif
            TypeRegistrySingleton::initialize_singleton();
            TypeRegistrySingleton::set_signatures_of_class(Monster::TypeIdClass(), Monster::get_signatures());
            TypeRegistrySingleton::set_signatures_of_class(MonsterArray::TypeIdClass(), MonsterArray::get_signatures());
            TypeRegistrySingleton::set_signatures_of_class(GameObject::TypeIdClass(), GameObject::get_signatures());

            FactoryManagerSingleton::initialize_singleton();
            MonsterFactory* mf = new MonsterFactory();
            FactoryManagerSingleton::add_factory(mf);
            MonsterArrayFactory* maf = new MonsterArrayFactory();
            FactoryManagerSingleton::add_factory(maf);
            GameObjectFactory* gof = new GameObjectFactory();
            FactoryManagerSingleton::add_factory(gof);
        }

        // ported from FooTests.cpp
        TEST_METHOD_CLEANUP(Cleanup)
        {
            FactoryManagerSingleton::destroy_singleton();
            TypeRegistrySingleton::destroy_singleton();

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

        TEST_METHOD(BasicTest)
        {
            Assert::IsTrue(true);
            GameObject go;
            
        }

        TEST_METHOD(UnnestedMonsterTest)
        {
            // Simple scope with one Monster type with only prescribed attributes
            {
                std::string jsonString = R"({
                    "MonsterScope" : 
                    {
                        "_class" : "Monster",
                        "monsterHP" : 10,
                        "monsterName" : "Joe"
                    }
                })";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                Scope* monsterScope = myScope->get_datum_by_key("MonsterScope")->get_scope_at(0);
                Assert::IsTrue(monsterScope->Is(Monster::TypeIdClass()));
                Assert::AreEqual(monsterScope->size(), 3_z);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterHP"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterHP"_s)->get_int_at(0), 10);
                Assert::AreEqual((*(Monster*)monsterScope).monsterHP, 10);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterName"_s)->get_type(), DatumType::String);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterName"_s)->get_string_at(0), "Joe"_s);
                Assert::AreEqual((*(Monster*)monsterScope).monsterName, "Joe"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Simple scope with three Monsters with prescribed and extra attributes
            {
                std::string jsonString = R"({
                    "MonsterScope" : 
                    {
                        "_class" : "Monster",
                        "monsterHP" : 10,
                        "monsterName" : "Joe"
                    },
                    "MonsterScope2" :
                    {
                        "_class" : "Monster",
                        "monsterHP" : 1,
                        "monsterName" : "Ghost Girl"
                    },
                    "MonsterScope3" :
                    {
                        "_class" : "Monster",
                        "monsterHP" : 100,
                        "monsterName" : "Nutcracker",
                        "weapon" : "Shotgun"
                    }
                })";
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 3_z);

                Scope* monsterScope = myScope->get_datum_by_key("MonsterScope")->get_scope_at(0);
                Assert::IsTrue(monsterScope->Is(Monster::TypeIdClass()));
                Assert::AreEqual(monsterScope->size(), 3_z);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterHP"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterHP"_s)->get_int_at(0), 10);
                Assert::AreEqual((*(Monster*)monsterScope).monsterHP, 10);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterName"_s)->get_type(), DatumType::String);
                Assert::AreEqual(monsterScope->get_datum_by_key("monsterName"_s)->get_string_at(0), "Joe"_s);
                Assert::AreEqual((*(Monster*)monsterScope).monsterName, "Joe"_s);

                Scope* monsterScope2 = myScope->get_datum_by_key("MonsterScope2")->get_scope_at(0);
                Assert::IsTrue(monsterScope2->Is(Monster::TypeIdClass()));
                Assert::AreEqual(monsterScope2->size(), 3_z);
                Assert::AreEqual(monsterScope2->get_datum_by_key("monsterHP"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(monsterScope2->get_datum_by_key("monsterHP"_s)->get_int_at(0), 1);
                Assert::AreEqual((*(Monster*)monsterScope2).monsterHP, 1);
                Assert::AreEqual(monsterScope2->get_datum_by_key("monsterName"_s)->get_type(), DatumType::String);
                Assert::AreEqual(monsterScope2->get_datum_by_key("monsterName"_s)->get_string_at(0), "Ghost Girl"_s);
                Assert::AreEqual((*(Monster*)monsterScope2).monsterName, "Ghost Girl"_s);

                Scope* monsterScope3 = myScope->get_datum_by_key("MonsterScope3")->get_scope_at(0);
                Assert::IsTrue(monsterScope3->Is(Monster::TypeIdClass()));
                Assert::AreEqual(monsterScope3->size(), 4_z);
                Assert::AreEqual(monsterScope3->get_datum_by_key("monsterHP"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(monsterScope3->get_datum_by_key("monsterHP"_s)->get_int_at(0), 100);
                Assert::AreEqual((*(Monster*)monsterScope3).monsterHP, 100);
                Assert::AreEqual(monsterScope3->get_datum_by_key("monsterName"_s)->get_type(), DatumType::String);
                Assert::AreEqual(monsterScope3->get_datum_by_key("monsterName"_s)->get_string_at(0), "Nutcracker"_s);
                Assert::AreEqual((*(Monster*)monsterScope3).monsterName, "Nutcracker"_s);
                Assert::AreEqual(monsterScope3->get_datum_by_key("weapon"_s)->get_type(), DatumType::String);
                Assert::AreEqual(monsterScope3->get_datum_by_key("weapon"_s)->get_string_at(0), "Shotgun"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

        TEST_METHOD(UnnestedMonsterArrayTest)
        {
            // Simple MonsterArray with no extra datums
            {
                std::string jsonString = R"-({
                    "MonsterArrayScope": {
                        "_class": "MonsterArray",
                        "intArray": [
                            1,
                            2,
                            3
                        ],
                        "floatArray": [
                            11.0,
                            12.0,
                            13.0
                        ],
                        "vecArray": [
                            "vec4(1, 2, 3, 4)",
                            "vec4(10, 2, 3, 4)",
                            "vec4(100, 2, 3, 4)"
                        ],
                        "matArray": [
                            "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                            "mat4x4((10, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                            "mat4x4((100, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))"
                        ],
                        "strArray": [
                            "string1",
                            "string2",
                            "string3"
                        ]
                    }
                })-";

                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                Scope* maScope = myScope->get_datum_by_key("MonsterArrayScope")->get_scope_at(0);
                Assert::IsTrue(maScope->Is(MonsterArray::TypeIdClass()));
                Assert::AreEqual(maScope->size(), 6_z);

                Assert::AreEqual(maScope->get_datum_by_key("intArray"_s)->get_type(), DatumType::Integer);
                Assert::AreEqual(maScope->get_datum_by_key("intArray"_s)->get_int_at(0), 1);
                Assert::AreEqual(maScope->get_datum_by_key("intArray"_s)->get_int_at(1), 2);
                Assert::AreEqual(maScope->get_datum_by_key("intArray"_s)->get_int_at(2), 3);
                Assert::AreEqual((*(MonsterArray*)maScope).intArray[0], 1);
                Assert::AreEqual((*(MonsterArray*)maScope).intArray[1], 2);
                Assert::AreEqual((*(MonsterArray*)maScope).intArray[2], 3);

                Assert::AreEqual(maScope->get_datum_by_key("floatArray"_s)->get_type(), DatumType::Float);
                Assert::AreEqual(maScope->get_datum_by_key("floatArray"_s)->get_float_at(0), 11_f);
                Assert::AreEqual(maScope->get_datum_by_key("floatArray"_s)->get_float_at(1), 12_f);
                Assert::AreEqual(maScope->get_datum_by_key("floatArray"_s)->get_float_at(2), 13_f);
                Assert::AreEqual((*(MonsterArray*)maScope).floatArray[0], 11_f);
                Assert::AreEqual((*(MonsterArray*)maScope).floatArray[1], 12_f);
                Assert::AreEqual((*(MonsterArray*)maScope).floatArray[2], 13_f);

                Assert::AreEqual(maScope->get_datum_by_key("vecArray"_s)->get_type(), DatumType::Vector4);
                Assert::AreEqual(maScope->get_datum_by_key("vecArray"_s)->get_vec4_at(0), glm::vec4(1,2,3,4));
                Assert::AreEqual(maScope->get_datum_by_key("vecArray"_s)->get_vec4_at(1), glm::vec4(10,2,3,4));
                Assert::AreEqual(maScope->get_datum_by_key("vecArray"_s)->get_vec4_at(2), glm::vec4(100,2,3,4));
                Assert::AreEqual((*(MonsterArray*)maScope).vecArray[0], glm::vec4(1,2,3,4));
                Assert::AreEqual((*(MonsterArray*)maScope).vecArray[1], glm::vec4(10,2,3,4));
                Assert::AreEqual((*(MonsterArray*)maScope).vecArray[2], glm::vec4(100,2,3,4));

                Assert::AreEqual(maScope->get_datum_by_key("matArray"_s)->get_type(), DatumType::Matrix4);
                Assert::AreEqual(maScope->get_datum_by_key("matArray"_s)->get_mat4_at(0), glm::mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
                Assert::AreEqual(maScope->get_datum_by_key("matArray"_s)->get_mat4_at(1), glm::mat4(10,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
                Assert::AreEqual(maScope->get_datum_by_key("matArray"_s)->get_mat4_at(2), glm::mat4(100,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
                Assert::AreEqual((*(MonsterArray*)maScope).matArray[0], glm::mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
                Assert::AreEqual((*(MonsterArray*)maScope).matArray[1], glm::mat4(10,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));
                Assert::AreEqual((*(MonsterArray*)maScope).matArray[2], glm::mat4(100,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16));

                Assert::AreEqual(maScope->get_datum_by_key("strArray"_s)->get_type(), DatumType::String);
                Assert::AreEqual(maScope->get_datum_by_key("strArray"_s)->get_string_at(0), "string1"_s);
                Assert::AreEqual(maScope->get_datum_by_key("strArray"_s)->get_string_at(1), "string2"_s);
                Assert::AreEqual(maScope->get_datum_by_key("strArray"_s)->get_string_at(2), "string3"_s);
                Assert::AreEqual((*(MonsterArray*)maScope).strArray[0], "string1"_s);
                Assert::AreEqual((*(MonsterArray*)maScope).strArray[1], "string2"_s);
                Assert::AreEqual((*(MonsterArray*)maScope).strArray[2], "string3"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Simple MonsterArray with one extra datum
            {
                std::string jsonString = R"-({
                    "MonsterArrayScope": {
                        "_class": "MonsterArray",
                        "intArray": [
                            1,
                            2,
                            3
                        ],
                        "floatArray": [
                            11.0,
                            12.0,
                            13.0
                        ],
                        "vecArray": [
                            "vec4(1, 2, 3, 4)",
                            "vec4(10, 2, 3, 4)",
                            "vec4(100, 2, 3, 4)"
                        ],
                        "matArray": [
                            "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                            "mat4x4((10, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))",
                            "mat4x4((100, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))"
                        ],
                        "strArray": [
                            "string1",
                            "string2",
                            "string3"
                        ],
                        "extraArray": [
                            "hello",
                            "what's",
                            "up",
                            "dog"
                        ]
                    }
                })-";

                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                Scope* maScope = myScope->get_datum_by_key("MonsterArrayScope")->get_scope_at(0);
                Assert::IsTrue(maScope->Is(MonsterArray::TypeIdClass()));
                Assert::AreEqual(maScope->size(), 7_z);

                Assert::AreEqual(maScope->get_datum_by_key("extraArray"_s)->get_type(), DatumType::String);
                Assert::AreEqual(maScope->get_datum_by_key("extraArray"_s)->get_string_at(0), "hello"_s);
                Assert::AreEqual(maScope->get_datum_by_key("extraArray"_s)->get_string_at(1), "what's"_s);
                Assert::AreEqual(maScope->get_datum_by_key("extraArray"_s)->get_string_at(2), "up"_s);
                Assert::AreEqual(maScope->get_datum_by_key("extraArray"_s)->get_string_at(3), "dog"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

        TEST_METHOD(UnnestedGOTest)
        {
            // Simple GO with no extra settings
            {
                std::string jsonString = R"({
                    "GameObject1" : 
                    {
                        "_class" : "GameObject"
                    }
                })";

                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                GameObject* go = myScope->get_datum_by_key("GameObject1")->get_scope_at(0)->As<GameObject>();
                Assert::AreEqual(go->localPosition(), Vector3(0, 0, 0));
                Assert::AreEqual(go->localEulerAngles(), Vector3(0, 0, 0));
                Assert::AreEqual(go->localScale(), Vector3(1, 1, 1));
                Assert::AreEqual(go->name(), "GameObject"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Simple GO with transform and name
            {
                std::string jsonString = R"-({
                    "GameObject1" : 
                    {
                        "_class" : "GameObject",
                        "mName" : "Okayu",
                        "mPosition" : "vec4(1, 2, 3, 1)",
                        "mRotation" : "vec4(1, 1, 1, 0)",
                        "mScale" : "vec4(0.5, 0.5, 0.5, 0)"
                    }
                })-";

                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                GameObject* go = myScope->get_datum_by_key("GameObject1")->get_scope_at(0)->As<GameObject>();
                Assert::AreEqual(go->localPosition(), Vector3(1, 2, 3));
                Assert::AreEqual(go->localEulerAngles(), Vector3(1, 1, 1));
                Assert::AreEqual(go->localScale(), Vector3(0.5, 0.5, 0.5));
                Assert::AreEqual(go->name(), "Okayu"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
            // Simple GO with transform and name and extra 
            {
                std::string jsonString = R"-({
                    "GameObject1" : 
                    {
                        "_class" : "GameObject",
                        "mName" : "Okayu",
                        "mPosition" : "vec4(1, 2, 3, 1)",
                        "mRotation" : "vec4(1, 1, 1, 0)",
                        "mScale" : "vec4(0.5, 0.5, 0.5, 0)",
                        "extraInt" : 1,
                        "extraString" : "Mogu mogu"
                    }
                })-";

                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                GameObject* go = myScope->get_datum_by_key("GameObject1")->get_scope_at(0)->As<GameObject>();
                Assert::AreEqual(go->localPosition(), Vector3(1, 2, 3));
                Assert::AreEqual(go->localEulerAngles(), Vector3(1, 1, 1));
                Assert::AreEqual(go->localScale(), Vector3(0.5, 0.5, 0.5));
                Assert::AreEqual(go->name(), "Okayu"_s);
                Assert::AreEqual(go->get_datum_by_key("extraInt")->get_int_at(0), 1);
                Assert::AreEqual(go->get_datum_by_key("extraString")->get_string_at(0), "Mogu mogu"_s);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

        TEST_METHOD(NestedGOTest)
        {
            // Nested scope tree with only names
            {
                std::string jsonString = R"-({
                "GameObject1": {
                    "_class": "GameObject",
                    "mName": "go",
                    "mChildren": [
                        {
                            "_class": "GameObject",
                            "mName": "go2",
                            "mChildren": [
                                {
                                    "_class": "GameObject",
                                    "mName": "go4",
                                    "mChildren": []
                                }
                            ]
                        },
                        {
                            "_class": "GameObject",
                            "mName": "go3",
                            "mChildren": []
                        }
                    ]
                }
            })-";
                // go
                //  |-> go2
                //       |-> go4
                //  |-> go3
                Scope* myScope = new Scope();
                ScopeWrapper* myWrapper = new ScopeWrapper(myScope);
                ScopeHandler* myHandler = new ScopeHandler();
                ParseCoordinator PC(myWrapper);
                PC.add_handler(myHandler);

                bool success = PC.deserialize_json_string(jsonString);

                Assert::IsTrue(success);
                Assert::AreEqual(myScope->size(), 1_z);

                GameObject* go = myScope->get_datum_by_key("GameObject1")->get_scope_at(0)->As<GameObject>();
                Assert::AreEqual(go->name(), "go"_s);
                Assert::AreEqual(go->childCount(), 2_z);
                GameObject* go2 = go->GetChild(0);
                Assert::AreEqual(go2->name(), "go2"_s);
                Assert::AreEqual(go2->childCount(), 1_z);
                GameObject* go3 = go->GetChild(1);
                Assert::AreEqual(go3->name(), "go3"_s);
                Assert::AreEqual(go3->childCount(), 0_z);
                GameObject* go4 = go2->GetChild(0);
                Assert::AreEqual(go4->name(), "go4"_s);
                Assert::AreEqual(go4->childCount(), 0_z);

                delete myHandler;
                delete myWrapper;
                delete myScope;
            }
        }

    private:
        // ported from FooTests.cpp
        inline static _CrtMemState _startMemState;
    };
}
