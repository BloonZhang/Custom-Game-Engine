#include "pch.h"
#include <iostream>
#include <string>
#include "CppUnitTest.h"
#include <filesystem>

#include "Definitions.h"
#include "TestDefinitions.h"

#include "EventQueueSingleton.hpp"

#include "GameObject.h"
#include "EventHandler.h"

#include "TestClasses/ThrowBoneEventHandler.h"
#include "TestClasses/Person.h"
#include "TestClasses/Dog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
    TEST_CLASS(EventTest)
    {
    public:
        // ported from FooTests.cpp
        TEST_METHOD_INITIALIZE(Initialize)
        {
#if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
#endif
            GameClock::initialize_singleton();
            EventQueueSingleton::initialize_singleton();
        }

        // ported from FooTests.cpp
        TEST_METHOD_CLEANUP(Cleanup)
        {
            EventQueueSingleton::destroy_singleton();
            GameClock::destroy_singleton();
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

        }

        TEST_METHOD(PersonAndDogTest)
        {
            // Simple test: person throwing a few bones
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;
                d.SubscribeToPerson(p);

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);

                p.ThrowBone("White", 10);
                p.ThrowBone("Blue", 100);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);

                p.ThrowBone("Black", 5);
                p.ThrowBone("Red", 7);

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);

                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 3_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "Black"_s);
                Assert::AreEqual(bonesEaten[2], "Red"_s);
            }
            // Simple test: unsubscribing from person
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;
                d.SubscribeToPerson(p);

                p.ThrowBone("White", 10);
                p.ThrowBone("Blue", 100);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);

                p.ThrowBoneEvent.UnsubscribeAll(&d);

                p.ThrowBone("Black", 5);
                p.ThrowBone("Red", 7);

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
            }
            // Simple test: three people throwing bones to two dogs
            {
                Person p1;
                Person p2;
                Person p3;
                Dog d1; // Only cares about p1, and p2
                Dog d2; // Cares about all three people
                std::vector<std::string> bonesEaten;
                d1.SubscribeToPerson(p1);
                d1.SubscribeToPerson(p2);
                d2.SubscribeToPerson(p1);
                d2.SubscribeToPerson(p2);
                d2.SubscribeToPerson(p3);

                p1.ThrowBone("White", 5);
                p2.ThrowBone("Black", 5);
                p3.ThrowBone("Red", 7);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d1.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 2_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "Black"_s);
                bonesEaten = d2.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 3_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "Black"_s);
                Assert::AreEqual(bonesEaten[2], "Red"_s);
            }
        }

        TEST_METHOD(EventEdgeCaseTest)
        {
            // Send an event with no subscribers
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);

                p.ThrowBone("White", 10);
                p.ThrowBone("Blue", 100);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);       
            }
            // dispatching empty queue
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;

                EventQueueSingleton::dispatch_event_queue();
                EventQueueSingleton::dispatch_event_queue();
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);
            }
            // spamming queue dispatches
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;
                d.SubscribeToPerson(p);

                p.ThrowBone("White", 10);
                EventQueueSingleton::dispatch_event_queue();
                EventQueueSingleton::dispatch_event_queue();
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
            }
            // Subscribe then unsubscribe
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;

                d.SubscribeToPerson(p);
                p.ThrowBoneEvent.UnsubscribeAll(&d);
                p.ThrowBone("White", 10);

                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);
            }
            // Subscribe then unsubscribe multiple dogs
            {
                Person p;
                Dog d1;
                Dog d2;
                Dog d3;
                std::vector<std::string> bonesEaten;

                d1.SubscribeToPerson(p);
                d2.SubscribeToPerson(p);
                d3.SubscribeToPerson(p);

                p.ThrowBone("White", 10);
                //EventQueueSingleton::dispatch_event_queue();

                p.ThrowBoneEvent.UnsubscribeAll(&d1);
                p.ThrowBoneEvent.UnsubscribeAll(&d2);
                p.ThrowBone("Blue", 5);
                //EventQueueSingleton::dispatch_event_queue();

                p.ThrowBoneEvent.UnsubscribeAll(&d3);
                p.ThrowBone("Black", 10);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d1.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                bonesEaten = d2.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                bonesEaten = d3.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 2_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "Blue"_s);
            }
            // Multiple events on same dog
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;

                d.SubscribeToPerson(p);
                d.SubscribeToPerson(p);

                p.ThrowBone("White", 10);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 2_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "White"_s);

                p.ThrowBoneEvent.UnsubscribeAll(&d);
                p.ThrowBone("White", 10);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 2_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "White"_s);
            }
            // Multiple dogs on same event
            {
                Person p;
                Dog d1;
                Dog d2;
                Dog d3;
                std::vector<std::string> bonesEaten;

                d1.SubscribeToPerson(p);
                d2.SubscribeToPerson(p);
                d3.SubscribeToPerson(p);
                d3.SetMaximumMass(1000);

                p.ThrowBone("White", 10);
                p.ThrowBone("Blue", 100);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d1.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                bonesEaten = d2.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                bonesEaten = d3.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 2_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
                Assert::AreEqual(bonesEaten[1], "Blue"_s);
            }
        
}

        TEST_METHOD(DelayTest)
        {
            // Call event on a delay
            {
                Person p;
                Dog d;
                std::vector<std::string> bonesEaten;
                d.SubscribeToPerson(p);

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);

                p.ThrowBoneWithDelay("White", 10, 100);
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 0_z);

                // this is very cringe, actually sleeping in a unit test. but that's ok
                long long startTime = GameClock::Current().Get();
                while (GameClock::Current().Get() < startTime + 200)
                {
                    continue;
                }
                EventQueueSingleton::dispatch_event_queue();

                bonesEaten = d.GetBonesEaten();
                Assert::AreEqual(bonesEaten.size(), 1_z);
                Assert::AreEqual(bonesEaten[0], "White"_s);
            }
        }

    private:
        // ported from FooTests.cpp
        inline static _CrtMemState _startMemState;
    };
}