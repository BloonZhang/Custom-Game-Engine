#include "pch.h"
#include <iostream>
#include <string>
#include "CppUnitTest.h"
#include <filesystem>

#include "Definitions.h"
#include "TestDefinitions.h"

#include "TypeRegistrySingleton.hpp"
#include "FactoryManagerSingleton.hpp"

#include "GameObject.h"
#include "TestClasses/TimeSum.h"
#include "TestClasses/ChildOfGameObject.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListWhile.h."
#include "TestClasses/ActionIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
    TEST_CLASS(GameObjectTest)
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
            TypeRegistrySingleton::set_signatures_of_class(GameObject::TypeIdClass(), GameObject::get_signatures());
            TypeRegistrySingleton::set_signatures_of_class(TimeSum::TypeIdClass(), TimeSum::get_signatures(), GameObject::TypeIdClass());
            TypeRegistrySingleton::set_signatures_of_class(ChildOfGameObject::TypeIdClass(), ChildOfGameObject::get_signatures(), GameObject::TypeIdClass());

            TypeRegistrySingleton::set_signatures_of_class(Action::TypeIdClass(), Action::get_signatures());
            TypeRegistrySingleton::set_signatures_of_class(ActionIncrement::TypeIdClass(), ActionIncrement::get_signatures(), Action::TypeIdClass());
            TypeRegistrySingleton::set_signatures_of_class(ActionList::TypeIdClass(), ActionList::get_signatures(), Action::TypeIdClass());
            TypeRegistrySingleton::set_signatures_of_class(ActionListWhile::TypeIdClass(), ActionListWhile::get_signatures(), ActionList::TypeIdClass());

            FactoryManagerSingleton::initialize_singleton();

            GameClock::initialize_singleton();
        }

        // ported from FooTests.cpp
        TEST_METHOD_CLEANUP(Cleanup)
        {
            GameClock::destroy_singleton();
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
            TimeSum ts;
        }

        TEST_METHOD(GOActionAugments)
        {
            // void adopt_action(Action*);
            {
                GameObject* go = new GameObject();
                ActionIncrement* act = new ActionIncrement();
                ActionIncrement* act2 = new ActionIncrement();
                ActionIncrement* act3 = new ActionIncrement();

                Assert::AreEqual(go->get_actions_datum()->size(), 0_z);
                go->adopt_action(act);
                go->adopt_action(act2);
                go->adopt_action(act3);
                Assert::AreEqual(go->get_actions_datum()->size(), 3_z);
                Assert::AreEqual((size_t)go->get_actions_datum()->get_scope_at(0), (size_t)act);
                Assert::AreEqual((size_t)go->get_actions_datum()->get_scope_at(1), (size_t)act2);
                Assert::AreEqual((size_t)go->get_actions_datum()->get_scope_at(2), (size_t)act3);

                delete go;
            }
        }

        TEST_METHOD(GOProperties)
        {
            // Datum* get_children_datum();
            {
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                Assert::AreEqual(go->get_children_datum(), go->get_datum_by_key("mChildren"));
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go);
                Assert::AreEqual(go->get_children_datum(), go->get_datum_by_key("mChildren"));

                delete go;
            }
            //const size_t childCount() const;
            {
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                Assert::AreEqual(go->childCount(), 0_z);
                Assert::AreEqual(go->childCount(), go->get_datum_by_key("mChildren")->size());
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go);
                Assert::AreEqual(go->childCount(), 3_z);
                Assert::AreEqual(go->childCount(), go->get_datum_by_key("mChildren")->size());
                
                delete go;
            }
            //const Vector3 localEulerAngles() const;
            {
                GameObject* go = new GameObject();
                Vector3 newRotation = Vector3(1, 1, 1);
                Assert::AreEqual(go->localEulerAngles(), Vector3(0, 0, 0));
                go->get_datum_by_key("mRotation")->set_vec4_at(0, glm::vec4(newRotation, 0));
                Assert::AreEqual(go->localEulerAngles(), newRotation);
                delete go;
            }
            //void setLocalEulerAngles(const Vector3&);
            {
                GameObject* go = new GameObject();
                Vector3 newRotation = Vector3(1, 1, 1);
                Assert::AreEqual(go->localEulerAngles(), Vector3(0, 0, 0));
                go->setLocalEulerAngles(newRotation);
                Assert::AreEqual(go->localEulerAngles(), newRotation);
                Assert::AreEqual(go->get_datum_by_key("mRotation")->get_vec4_at(0), glm::vec4(newRotation, 0));
                delete go;
            }
            //const Vector3 localPosition() const;
            {
                GameObject* go = new GameObject();
                Vector3 newPosition = Vector3(1, 2, 3);
                Assert::AreEqual(go->localPosition(), Vector3(0, 0, 0));
                go->get_datum_by_key("mPosition")->set_vec4_at(0, glm::vec4(newPosition, 1));
                Assert::AreEqual(go->localPosition(), newPosition);
                delete go;
            }
            //void setLocalPosition(const Vector3&);
            {
                GameObject* go = new GameObject();
                Vector3 newPosition = Vector3(1, 2, 3);
                Assert::AreEqual(go->localPosition(), Vector3(0, 0, 0));
                go->setLocalPosition(newPosition);
                Assert::AreEqual(go->localPosition(), newPosition);
                Assert::AreEqual(go->get_datum_by_key("mPosition")->get_vec4_at(0), glm::vec4(newPosition, 1));
                delete go;
            }
            //const Vector3 localScale() const;
            {
                GameObject* go = new GameObject();
                Vector3 newScale = Vector3(0.5, 0.5, 0.5);
                Assert::AreEqual(go->localScale(), Vector3(1, 1, 1));
                go->get_datum_by_key("mScale")->set_vec4_at(0, glm::vec4(newScale, 1));
                Assert::AreEqual(go->localScale(), newScale);
                delete go;
            }
            //void setLocalScale(const Vector3&);
            {
                GameObject* go = new GameObject();
                Vector3 newScale = Vector3(0.5, 0.5, 0.5);
                Assert::AreEqual(go->localScale(), Vector3(1, 1, 1));
                go->setLocalScale(newScale);
                Assert::AreEqual(go->localScale(), newScale);
                Assert::AreEqual(go->get_datum_by_key("mScale")->get_vec4_at(0), glm::vec4(newScale, 0));
                delete go;
            }
            //GameObject* const parent() const;
            {
                // go
                //  |-> go2
                //       |-> go4
                //  |-> go3
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go2);

                Assert::AreEqual((void*)go->parent(), (void*)nullptr);
                Assert::AreEqual((void*)go2->parent(), (void*)go);
                Assert::AreEqual((void*)go3->parent(), (void*)go);
                Assert::AreEqual((void*)go4->parent(), (void*)go2);

                delete go;
            }
            //GameObject* const root() const;
            {
                // go
                //  |-> go2
                //       |-> go4
                //  |-> go3
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go2);

                Assert::AreEqual((void*)go->root(), (void*)go);
                Assert::AreEqual((void*)go2->root(), (void*)go);
                Assert::AreEqual((void*)go3->root(), (void*)go);
                Assert::AreEqual((void*)go4->root(), (void*)go);

                delete go;
            }
            //const Transform localTransform() const;
            {
                GameObject* go = new GameObject();
                Vector3 newPosition = Vector3(1, 2, 3);
                Vector3 newRotation = Vector3(1, 1, 1);
                Vector3 newScale = Vector3(0.5, 0.5, 0.5);

                Transform myTransform =  go->localTransform();
                Assert::AreEqual(myTransform.position, Vector3(0, 0, 0));
                Assert::AreEqual(myTransform.rotation, Vector3(0, 0, 0));
                Assert::AreEqual(myTransform.scale, Vector3(1, 1, 1));

                go->setLocalPosition(newPosition);
                go->setLocalEulerAngles(newRotation);
                go->setLocalScale(newScale);

                myTransform = go->localTransform();
                Assert::AreEqual(myTransform.position, newPosition);
                Assert::AreEqual(myTransform.rotation, newRotation);
                Assert::AreEqual(myTransform.scale, newScale);

                delete go;
            }
            //void setLocalTransform(const Transform&);
            {
                GameObject* go = new GameObject();
                Vector3 newPosition = Vector3(1, 2, 3);
                Vector3 newRotation = Vector3(1, 1, 1);
                Vector3 newScale = Vector3(0.5, 0.5, 0.5);
                Transform newTransform;
                newTransform.position = newPosition;
                newTransform.rotation = newRotation;
                newTransform.scale = newScale;

                Transform myTransform =  go->localTransform();
                Assert::AreEqual(myTransform.position, Vector3(0, 0, 0));
                Assert::AreEqual(myTransform.rotation, Vector3(0, 0, 0));
                Assert::AreEqual(myTransform.scale, Vector3(1, 1, 1));

                go->setLocalTransform(newTransform);
                myTransform = go->localTransform();
                Assert::AreEqual(myTransform.position, newPosition);
                Assert::AreEqual(go->get_datum_by_key("mPosition")->get_vec4_at(0), Vector4(newPosition, 1));
                Assert::AreEqual(myTransform.rotation, newRotation);
                Assert::AreEqual(go->get_datum_by_key("mRotation")->get_vec4_at(0), Vector4(newRotation, 0));
                Assert::AreEqual(myTransform.scale, newScale);
                Assert::AreEqual(go->get_datum_by_key("mScale")->get_vec4_at(0), Vector4(newScale, 0));

                delete go;
            }
            //const std::string name() const;
            {
                GameObject* go = new GameObject();
                Assert::AreEqual(go->name(), "GameObject"_s);
                go->get_datum_by_key("mName")->set_string_at(0, "Okayu");
                Assert::AreEqual(go->name(), "Okayu"_s);
                delete go;
            }
            //void setName(const std::string&);
            {
                GameObject* go = new GameObject();
                Assert::AreEqual(go->name(), "GameObject"_s);
                go->setName("Okayu");
                Assert::AreEqual(go->name(), "Okayu"_s);
                Assert::AreEqual(go->get_datum_by_key("mName")->get_string_at(0), "Okayu"_s);
                delete go;
            }
        }

        TEST_METHOD(GOMethods)
        {
            //const GameObject* GetChild(size_t) const;
            //GameObject* GetChild(size_t);
            {
                // go
                //  |-> go2
                //       |-> go4
                //  |-> go3
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go2);

                GameObject* child = nullptr;
                child = go->GetChild(0);
                Assert::AreEqual((void*)child, (void*)go2);
                child = go->GetChild(1);
                Assert::AreEqual((void*)child, (void*)go3);
                child = go2->GetChild(0);
                Assert::AreEqual((void*)child, (void*)go4);

                const GameObject* goConst = go;
                const GameObject* goConst2 = go2;
                child = go->GetChild(0);
                Assert::AreEqual((void*)child, (void*)go2);
                child = go->GetChild(1);
                Assert::AreEqual((void*)child, (void*)go3);
                child = go2->GetChild(0);
                Assert::AreEqual((void*)child, (void*)go4);

                Assert::ExpectException<std::out_of_range>([&]() { go->GetChild(2); });
                Assert::ExpectException<std::out_of_range>([&]() { goConst->GetChild(2); });
                delete go;
            }
            //void SetParent(GameObject*);
            {
                GameObject* go = new GameObject();
                GameObject* go2 = new GameObject();
                GameObject* go3 = new GameObject();
                GameObject* go4 = new GameObject();
                Assert::AreEqual(go->childCount(), 0_z);
                Assert::AreEqual(go2->childCount(), 0_z);
                Assert::AreEqual(go3->childCount(), 0_z);
                Assert::AreEqual(go4->childCount(), 0_z);
                go2->SetParent(go);
                go3->SetParent(go);
                go4->SetParent(go);
                Assert::AreEqual(go->childCount(), 3_z);
                Assert::AreEqual(go2->childCount(), 0_z);
                Assert::AreEqual(go3->childCount(), 0_z);
                Assert::AreEqual(go4->childCount(), 0_z);
                go4->SetParent(go2);
                Assert::AreEqual(go->childCount(), 2_z);
                Assert::AreEqual(go2->childCount(), 1_z);
                Assert::AreEqual(go3->childCount(), 0_z);
                Assert::AreEqual(go4->childCount(), 0_z);

                Scope* child = go->get_datum_by_key("mChildren")->get_scope_at(0);
                Assert::AreEqual((void*)child, (void*)go2);
                child = go->get_datum_by_key("mChildren")->get_scope_at(1);
                Assert::AreEqual((void*)child, (void*)go3);
                child = go2->get_datum_by_key("mChildren")->get_scope_at(0);
                Assert::AreEqual((void*)child, (void*)go4);

                Assert::ExpectException<std::invalid_argument>([&]() { go->SetParent(go); });
                Assert::ExpectException<std::invalid_argument>([&]() { go->SetParent(go4); });

                delete go;
            }
        }

        TEST_METHOD(UpdateTest)
        {
            // update non-nested TimeSum
            {
                GameTime deltaTime = CreateElapsedTime(17);

                //GameTime deltaTime = GameTime(17);
                Vector3 newVelocity = Vector3(1, 2, 0);

                TimeSum* ts = new TimeSum();
                GameObject* go = ts;

                Assert::AreEqual(ts->totalTime, (long long)0);

                ts->velocity = newVelocity;
                for (int i = 0; i < 60; ++i)
                {
                    go->Update(deltaTime);
                }

                Assert::AreEqual(ts->totalTime, (long long)(1020));
                Assert::AreEqual(go->localPosition(), Vector3(60, 120, 0));

                delete ts;
            }
            // update nested TimeSum
            {
                GameTime deltaTime = CreateElapsedTime(17);

                Vector3 newVelocity = Vector3(1, 2, 0);

                TimeSum* ts = new TimeSum();
                TimeSum* ts2 = new TimeSum();
                TimeSum* ts3 = new TimeSum();
                GameObject* go = ts;
                ts2->SetParent(go);
                ts3->SetParent(ts2);

                Assert::AreEqual(ts->totalTime, (long long)(0_f));
                Assert::AreEqual(ts2->totalTime, (long long)(0_f));
                Assert::AreEqual(ts3->totalTime, (long long)(0_f));

                ts->velocity = newVelocity;
                ts2->velocity = newVelocity;
                ts3->velocity = Vector3(0,0,0);
                for (int i = 0; i < 60; ++i)
                {
                    go->Update(deltaTime);
                }

                Assert::AreEqual(ts->totalTime, (long long)(1020));
                Assert::AreEqual(go->localPosition(), Vector3(60, 120, 0));
                Assert::AreEqual(ts2->totalTime, (long long)(1020));
                Assert::AreEqual(ts2->localPosition(), Vector3(60, 120, 0));
                Assert::AreEqual(ts3->totalTime, (long long)(1020));
                Assert::AreEqual(ts3->localPosition(), Vector3(0, 0, 0));
                delete ts;
            }
        }

        TEST_METHOD(ActionIncrementTest)
        {
            // Update single ActionIncrement
            {
                GameTime deltaTime = CreateElapsedTime(17);

                ChildOfGameObject* go = new ChildOfGameObject();
                Assert::AreEqual(go->get_int(), 0);

                ActionIncrement* act = new ActionIncrement();
                act->set_name_of_target_datum("mInt");
                act->set_increment_amount(100);
                go->adopt_action(act);

                go->Update(deltaTime);
                go->Update(deltaTime);
                go->Update(deltaTime);

                Assert::AreEqual(go->get_int(), 300);

                delete go;
            }
            // Update multiple ActionIncrement
            {
                GameTime deltaTime = CreateElapsedTime(17);

                ChildOfGameObject* go = new ChildOfGameObject();
                Assert::AreEqual(go->get_int(), 0);

                ActionIncrement* act = new ActionIncrement();
                act->set_name_of_target_datum("mInt");
                act->set_increment_amount(100);
                go->adopt_action(act);
                ActionIncrement* act2 = new ActionIncrement();
                act2->set_name_of_target_datum("mInt");
                act2->set_increment_amount(100);
                go->adopt_action(act2);
                ActionIncrement* act3 = new ActionIncrement();
                act3->set_name_of_target_datum("mInt");
                act3->set_increment_amount(100);
                go->adopt_action(act3);

                go->Update(deltaTime);
                go->Update(deltaTime);
                go->Update(deltaTime);

                Assert::AreEqual(go->get_int(), 900);

                delete go;
            }
            // Nested ActionIncrement
            {
                GameTime deltaTime = CreateElapsedTime(17);

                ChildOfGameObject* go = new ChildOfGameObject();
                Assert::AreEqual(go->get_int(), 0);
                ChildOfGameObject* go2 = new ChildOfGameObject();
                Assert::AreEqual(go2->get_int(), 0);
                go2->SetParent(go);

                ActionIncrement* act = new ActionIncrement();
                act->set_name_of_target_datum("mInt");
                act->set_increment_amount(100);
                go->adopt_action(act);
                ActionIncrement* act2 = new ActionIncrement();
                act2->set_name_of_target_datum("mInt");
                act2->set_increment_amount(100);
                go2->adopt_action(act2);
                ActionIncrement* act3 = new ActionIncrement();
                act3->set_name_of_target_datum("mInt");
                act3->set_increment_amount(100);
                go2->adopt_action(act3);

                go->Update(deltaTime);
                go->Update(deltaTime);
                go->Update(deltaTime);

                Assert::AreEqual(go->get_int(), 300);
                Assert::AreEqual(go2->get_int(), 600);

                delete go;
            }
        }

        TEST_METHOD(ActionListTest)
        {
            {
                GameTime deltaTime = CreateElapsedTime(17);

                ChildOfGameObject* go = new ChildOfGameObject();
                ActionListWhile* actListW = new ActionListWhile();

                ActionIncrement* preamble = new ActionIncrement();
                preamble->set_name_of_target_datum("mCondition");
                preamble->set_increment_amount(10);
                actListW->adopt_action_as_preamble(preamble);

                ActionIncrement* increment = new ActionIncrement();
                increment->set_name_of_target_datum("mCondition");
                increment->set_increment_amount(-1);
                actListW->adopt_action_as_increment(increment);

                ActionIncrement* act = new ActionIncrement();
                act->set_name_of_target_datum("mInt");
                act->set_increment_amount(50);
                actListW->add_action_to_list(act);

                go->adopt_action(actListW);

                Assert::AreEqual(go->get_int(), 0);
                go->Update(deltaTime);
                Assert::AreEqual(go->get_int(), 500);
                go->Update(deltaTime);
                go->Update(deltaTime);
                Assert::AreEqual(go->get_int(), 1500);

                delete go;
            }
        }

    private:
        // ported from FooTests.cpp
        inline static _CrtMemState _startMemState;
    };
}