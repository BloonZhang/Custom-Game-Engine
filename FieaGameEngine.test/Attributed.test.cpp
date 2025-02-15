#include "pch.h"
#include <iostream>
#include "CppUnitTest.h"

#include "RTTI.h"
#include "Attributed.h"
#include "TypeRegistrySingleton.hpp"
#include "TestClasses/Elf.h"
#include "TestClasses/Mage.h"
#include "TestClasses/WhiteMage.h"
#include "Definitions.h"
#include "TestDefinitions.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

namespace Fiea::GameEngine::test
{
	TEST_CLASS(AttributedTest)
	{
	public:
		// ported from FooTests.cpp
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			// Initialize our type registry
			TypeRegistrySingleton::initialize_singleton();
			TypeRegistrySingleton::Instance->set_signatures_of_class(Elf::TypeIdClass(), Elf::get_signatures());
			TypeRegistrySingleton::Instance->set_signatures_of_class(Mage::TypeIdClass(), Mage::get_signatures());
			TypeRegistrySingleton::Instance->set_signatures_of_class(WhiteMage::TypeIdClass(), WhiteMage::get_signatures(), Mage::TypeIdClass());
		}
		// ported from FooTests.cpp
		TEST_METHOD_CLEANUP(Cleanup)
		{
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

		TEST_METHOD(TypeRegistry)
		{
			size_t myClassId1 = 1;
			size_t myClassId2 = 10001;
			size_t myClassThatInherits1 = 500;
			size_t myClassIdInvalid = 2;
			std::vector<Signature> mySignatures1 =
			{
				{"mogu mogu", DatumType::Integer, 1, 0},
				{"okayu", DatumType::Float, 1, 4}
			};
			std::vector<Signature> mySignatures2 =
			{
				{"grr grr", DatumType::String, 1, 0},
				{"tummy", DatumType::Matrix4, 1, 4},
				{"ready steady go", DatumType::Integer, 5, 8}
			};
			std::vector<Signature> mySignaturesThatInherts1 =
			{
				{"cat", DatumType::Vector4, 1, 0}
			};
			std::vector<Signature> mySignaturesThatInherts1PlusSubclass =
			{
				{"mogu mogu", DatumType::Integer, 1, 0},
				{"okayu", DatumType::Float, 1, 4},
				{"cat", DatumType::Vector4, 1, 0}
			};
			// Error handling for TypeRegistry
			{
				/*
				Assert::ExpectException<std::out_of_range>([&]()
					{
						TypeRegistrySingleton::Instance->set_signatures_of_class(myClassId1, mySignatures1);
					});
				Assert::ExpectException<std::out_of_range>([&]()
					{
						TypeRegistrySingleton::Instance->get_signatures_of_class(myClassId1);
					});
					*/
				Assert::ExpectException<std::invalid_argument>([&]()
					{
						TypeRegistrySingleton::initialize_singleton();
					});
				TypeRegistrySingleton::set_signatures_of_class(myClassId1, mySignatures1);
				TypeRegistrySingleton::set_signatures_of_class(myClassId2, mySignatures2);
				TypeRegistrySingleton::set_signatures_of_class(myClassId2, mySignatures2);
				Assert::ExpectException<std::out_of_range>([&]()
					{
						TypeRegistrySingleton::get_signatures_of_class(myClassIdInvalid);
					});
			}
			// Actual functionality for TypeRegistry
			{
				TypeRegistrySingleton::set_signatures_of_class(myClassId1, mySignatures1);
				TypeRegistrySingleton::set_signatures_of_class(myClassId2, mySignatures2);
				Assert::AreEqual(TypeRegistrySingleton::get_signatures_of_class(myClassId1), mySignatures1);
				Assert::AreEqual(TypeRegistrySingleton::get_signatures_of_class(myClassId2), mySignatures2);
				Assert::AreEqual(TypeRegistrySingleton::get_signatures_of_class(myClassId1), mySignatures1);
				Assert::AreEqual(TypeRegistrySingleton::get_signatures_of_class(myClassId2), mySignatures2);
				TypeRegistrySingleton::set_signatures_of_class(myClassThatInherits1, mySignaturesThatInherts1, myClassId1);
				Assert::AreEqual(TypeRegistrySingleton::get_signatures_of_class(myClassThatInherits1), mySignaturesThatInherts1PlusSubclass);
				std::vector<Signature> test = TypeRegistrySingleton::get_signatures_of_class(myClassThatInherits1);
			}
		}

		TEST_METHOD(MageTest)
		{
			// default constructor
			{
				Mage myMage;
				Assert::AreEqual(myMage.get_datum_by_key("this")->get_RTTI_at(0), reinterpret_cast<RTTI*>(&myMage));
				Assert::AreEqual(myMage.get_datum_by_key("mName")->get_string_at(0), myMage.mName);
				Assert::AreSame(myMage.get_datum_by_key("mName")->get_string_at(0), myMage.mName);
				Assert::AreEqual(myMage.get_datum_by_key("mHealth")->get_int_at(0), myMage.mHealth);
				Assert::AreSame(myMage.get_datum_by_key("mHealth")->get_int_at(0), myMage.mHealth);
				Assert::AreEqual(myMage.get_datum_by_key("mSpells")->get_string_at(0), myMage.mSpells[0]);
				Assert::AreSame(myMage.get_datum_by_key("mSpells")->get_string_at(0), myMage.mSpells[0]);
				Assert::AreEqual(myMage.get_datum_by_key("mSpells")->get_string_at(4), myMage.mSpells[4]);
				Assert::AreEqual(myMage.get_datum_by_key("mSpells")->get_string_at(4), "Healing"_s);
				Assert::AreSame(myMage.get_datum_by_key("mSpells")->get_string_at(4), myMage.mSpells[4]);
			}
			// Two-way binding
			{
				Mage myMage;
				std::string username1 = "iobgtanuki";
				std::string username2 = "bloonz";
				myMage.mName = username1;
				Assert::AreEqual(myMage.mName, username1);
				Assert::AreEqual(myMage.get_datum_by_key("mName")->get_string_at(0), username1);
				myMage.get_datum_by_key("mName")->set_string_at(0, username2);
				Assert::AreEqual(myMage.mName, username2);
				Assert::AreEqual(myMage.get_datum_by_key("mName")->get_string_at(0), username2);
				
				--myMage.mHealth;
				Assert::AreEqual(myMage.get_datum_by_key("mHealth")->get_int_at(0), myMage.mHealth);
				myMage.get_datum_by_key("mHealth")->set_int_at(0, myMage.get_datum_by_key("mHealth")->get_int_at(0) - 1);
				Assert::AreEqual(myMage.get_datum_by_key("mHealth")->get_int_at(0), myMage.mHealth);
			}
			// Order of addition
			{
				Mage myMage;
				Assert::AreEqual(&myMage[0], myMage.get_datum_by_key("this"));
				Assert::AreEqual(&myMage[1], myMage.get_datum_by_key("mName"));
				Assert::AreEqual(&myMage[2], myMage.get_datum_by_key("mHealth"));
				Assert::AreEqual(&myMage[3], myMage.get_datum_by_key("mSpells"));
			}
		}

		TEST_METHOD(ElfTest)
		{
			// Default constructor
			{
				Elf myElf;
				Assert::AreEqual(myElf.get_datum_by_key("this")->get_RTTI_at(0), reinterpret_cast<RTTI*>(&myElf));
				Assert::AreEqual(myElf.get_datum_by_key("mAge")->get_int_at(0), myElf.mAge);
				Assert::AreSame(myElf.get_datum_by_key("mAge")->get_int_at(0), myElf.mAge);
				Assert::AreEqual(myElf.get_datum_by_key("internalDatum")->size(), 0_z);
			}
			// Internal Datum via signature
			{
				Elf myElf;
				Assert::AreEqual(myElf.get_datum_by_key("internalDatum")->size(), 0_z);
				Scope* externalScope = new Elf();
				Scope* externalScope2 = new Elf();
				myElf.add_empty_scope("internalDatum");
				myElf.adopt_scope(externalScope, "internalDatum");
				myElf.adopt_scope(externalScope2, "internalDatum");
				Assert::AreEqual(myElf.get_datum_by_key("internalDatum")->size(), 3_z);
			}
			// Two-way binding
			{
				Elf myElf;
				myElf.mAge = 1500;
				Assert::AreEqual(myElf.get_datum_by_key("mAge")->get_int_at(0), myElf.mAge);
				myElf.get_datum_by_key("mAge")->set_int_at(0, 2000);
				Assert::AreEqual(myElf.get_datum_by_key("mAge")->get_int_at(0), myElf.mAge);
			}
			// Order of addition
			{
				Elf myElf;
				Assert::AreEqual(&myElf[0], myElf.get_datum_by_key("this"));
				Assert::AreEqual(&myElf[1], myElf.get_datum_by_key("mAge"));
			}
		}

		TEST_METHOD(WhiteMageTest)
		{
			// default constructor
			{
				WhiteMage myWhiteMage;
				Assert::AreEqual(myWhiteMage.get_datum_by_key("this")->get_RTTI_at(0), reinterpret_cast<RTTI*>(&myWhiteMage));
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), myWhiteMage.mName);
				Assert::AreSame(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), myWhiteMage.mName);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
				Assert::AreSame(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpells")->get_string_at(0), myWhiteMage.mSpells[0]);
				Assert::AreSame(myWhiteMage.get_datum_by_key("mSpells")->get_string_at(0), myWhiteMage.mSpells[0]);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpells")->get_string_at(4), myWhiteMage.mSpells[4]);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpells")->get_string_at(4), "Healing"_s);
				Assert::AreSame(myWhiteMage.get_datum_by_key("mSpells")->get_string_at(4), myWhiteMage.mSpells[4]);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), myWhiteMage.mSpecialtySpell);
				Assert::AreSame(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), myWhiteMage.mSpecialtySpell);
			}
			// Two-way binding
			{
				WhiteMage myWhiteMage;
				std::string username1 = "iobgtanuki";
				std::string username2 = "bloonz";
				myWhiteMage.mName = username1;
				Assert::AreEqual(myWhiteMage.mName, username1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
				myWhiteMage.get_datum_by_key("mName")->set_string_at(0, username2);
				Assert::AreEqual(myWhiteMage.mName, username2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username2);

				--myWhiteMage.mHealth;
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
				myWhiteMage.get_datum_by_key("mHealth")->set_int_at(0, myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0) - 1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);

				std::string spell1 = "Heavenly Blessing";
				std::string spell2 = "Wrath of God";
				myWhiteMage.mSpecialtySpell = spell1;
				Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
				myWhiteMage.get_datum_by_key("mSpecialtySpell")->set_string_at(0, spell2);
				Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell2);
			}
			// Order of addition
			{
				WhiteMage myWhiteMage;
				Assert::AreEqual(&myWhiteMage[0], myWhiteMage.get_datum_by_key("this"));
				Assert::AreEqual(&myWhiteMage[1], myWhiteMage.get_datum_by_key("mName"));
				Assert::AreEqual(&myWhiteMage[2], myWhiteMage.get_datum_by_key("mHealth"));
				Assert::AreEqual(&myWhiteMage[3], myWhiteMage.get_datum_by_key("mSpells"));
				Assert::AreEqual(&myWhiteMage[4], myWhiteMage.get_datum_by_key("mSpecialtySpell"));
				Assert::ExpectException<std::out_of_range>([&myWhiteMage]() { myWhiteMage[5]; });
			}
		}
		 
		TEST_METHOD(PrescribedAttributeTest)
		{
			Mage myMage;
			std::string auxiliaryKey = "mHat";
			Assert::IsTrue(myMage.is_attribute("this"));
			Assert::IsTrue(myMage.is_attribute("mName"));
			Assert::IsTrue(myMage.is_attribute("mHealth"));
			Assert::IsTrue(myMage.is_attribute("mSpells"));
			Assert::IsTrue(myMage.is_prescribed_attribute("this"));
			Assert::IsTrue(myMage.is_prescribed_attribute("mName"));
			Assert::IsTrue(myMage.is_prescribed_attribute("mHealth"));
			Assert::IsTrue(myMage.is_prescribed_attribute("mSpells"));

			Assert::IsFalse(myMage.is_prescribed_attribute(auxiliaryKey));
			myMage.add_auxiliary_attribute(auxiliaryKey);
			Assert::IsFalse(myMage.is_prescribed_attribute(auxiliaryKey));
		}

		TEST_METHOD(AuxiliaryAttributeTest)
		{
			Mage myMage;

			Assert::IsFalse(myMage.is_auxiliary_attribute("this"));
			Assert::IsFalse(myMage.is_auxiliary_attribute("mName"));
			Assert::IsFalse(myMage.is_auxiliary_attribute("mHealth"));
			Assert::IsFalse(myMage.is_auxiliary_attribute("mSpells"));

			std::string auxiliaryKey = "mHat";
			std::string nameOfHat = "Bot Dogger";
			Assert::IsFalse(myMage.is_attribute("mHat"));
			Assert::IsFalse(myMage.is_prescribed_attribute("mHat"));
			Assert::IsFalse(myMage.is_auxiliary_attribute("mHat"));
			myMage.add_auxiliary_attribute(auxiliaryKey)->push_string_back(nameOfHat);
			Assert::IsTrue(myMage.is_attribute("mHat"));
			Assert::IsFalse(myMage.is_prescribed_attribute("mHat"));
			Assert::IsTrue(myMage.is_auxiliary_attribute("mHat"));

			Assert::ExpectException<std::invalid_argument>([&myMage]() { myMage.add_auxiliary_attribute("mName"); });
		}

		TEST_METHOD(CopyConstructor)
		{
			WhiteMage myWhiteMage;
			std::string username1 = "iobgtanuki";
			std::string username2 = "bloonz";
			int damagedHealth = 5;
			int deadHealth = 0;
			std::string spell1 = "Heavenly Blessing";
			std::string spell2 = "Wrath of God";
			std::string auxiliaryKey = "mHat";
			std::string nameOfHat1 = "Bot Dogger";
			std::string nameOfHat2 = "Tsar Platinum";
			myWhiteMage.mName = username1;
			myWhiteMage.mHealth = damagedHealth;
			myWhiteMage.mSpecialtySpell = spell1;
			myWhiteMage.add_auxiliary_attribute(auxiliaryKey)->push_string_back(nameOfHat1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			WhiteMage myWhiteMage2(myWhiteMage);
			Assert::AreEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage[1].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage[2].get_int_at(0));
			Assert::AreEqual(myWhiteMage2[3].get_string_at(2), myWhiteMage[3].get_string_at(2));
			Assert::AreEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage[4].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			myWhiteMage.mName = username2;
			myWhiteMage.mHealth = deadHealth;
			myWhiteMage.mSpecialtySpell = spell2;
			myWhiteMage[auxiliaryKey].set_string_at(0, nameOfHat2);
			Assert::AreNotEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage[1].get_string_at(0));
			Assert::AreNotEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage[2].get_int_at(0));
			Assert::AreNotEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage[4].get_string_at(0));
			Assert::AreNotEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);
		}

		TEST_METHOD(MoveConstructor)
		{
			WhiteMage myWhiteMage;
			std::string username1 = "iobgtanuki";
			std::string username2 = "bloonz";
			int damagedHealth = 5;
			int deadHealth = 0;
			std::string spell1 = "Heavenly Blessing";
			std::string spell2 = "Wrath of God";
			std::string auxiliaryKey = "mHat";
			std::string nameOfHat1 = "Bot Dogger";
			std::string nameOfHat2 = "Tsar Platinum";
			myWhiteMage.mName = username1;
			myWhiteMage.mHealth = damagedHealth;
			myWhiteMage.mSpecialtySpell = spell1;
			myWhiteMage.add_auxiliary_attribute(auxiliaryKey)->push_string_back(nameOfHat1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			WhiteMage myWhiteMage1Copy(myWhiteMage);
			WhiteMage myWhiteMage2(std::move(myWhiteMage));
			Assert::AreEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage1Copy[1].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage1Copy[2].get_int_at(0));
			Assert::AreEqual(myWhiteMage2[3].get_string_at(2), myWhiteMage1Copy[3].get_string_at(2));
			Assert::AreEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage1Copy[4].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage1Copy[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);
		}

		TEST_METHOD(AssignmentOperator)
		{
			WhiteMage myWhiteMage;
			std::string username1 = "iobgtanuki";
			std::string username2 = "bloonz";
			int damagedHealth = 5;
			int deadHealth = 0;
			std::string spell1 = "Heavenly Blessing";
			std::string spell2 = "Wrath of God";
			std::string auxiliaryKey = "mHat";
			std::string nameOfHat1 = "Bot Dogger";
			std::string nameOfHat2 = "Tsar Platinum";
			std::string auxiliaryKey2 = "mPet";
			std::string nameOfPet2 = "Dog";
			myWhiteMage.mName = username1;
			myWhiteMage.mHealth = damagedHealth;
			myWhiteMage.mSpecialtySpell = spell1;
			myWhiteMage.add_auxiliary_attribute(auxiliaryKey)->push_string_back(nameOfHat1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			WhiteMage myWhiteMage2; 
			myWhiteMage2.mName = username2;
			myWhiteMage2.mHealth = deadHealth;
			myWhiteMage2.mSpecialtySpell = spell2;
			myWhiteMage2.add_datum(auxiliaryKey2)->push_string_back(nameOfPet2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), deadHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey2)->get_string_at(0), nameOfPet2);

			myWhiteMage2 = myWhiteMage;
			Assert::AreEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage[1].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage[2].get_int_at(0));
			Assert::AreEqual(myWhiteMage2[3].get_string_at(2), myWhiteMage[3].get_string_at(2));
			Assert::AreEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage[4].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey2), (Datum*)nullptr);

			myWhiteMage.mName = username2;
			myWhiteMage.mHealth = deadHealth;
			myWhiteMage.mSpecialtySpell = spell2;
			myWhiteMage[auxiliaryKey].set_string_at(0, nameOfHat2);
			Assert::AreNotEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage[1].get_string_at(0));
			Assert::AreNotEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage[2].get_int_at(0));
			Assert::AreNotEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage[4].get_string_at(0));
			Assert::AreNotEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			myWhiteMage2 = myWhiteMage2;
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			WhiteMage myWhiteMage;
			std::string username1 = "iobgtanuki";
			std::string username2 = "bloonz";
			int damagedHealth = 5;
			int deadHealth = 0;
			std::string spell1 = "Heavenly Blessing";
			std::string spell2 = "Wrath of God";
			std::string auxiliaryKey = "mHat";
			std::string nameOfHat1 = "Bot Dogger";
			std::string nameOfHat2 = "Tsar Platinum";
			std::string auxiliaryKey2 = "mPet";
			std::string nameOfPet2 = "Dog";
			myWhiteMage.mName = username1;
			myWhiteMage.mHealth = damagedHealth;
			myWhiteMage.mSpecialtySpell = spell1;
			myWhiteMage.add_auxiliary_attribute(auxiliaryKey)->push_string_back(nameOfHat1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);

			WhiteMage myWhiteMage2; 
			myWhiteMage2.mName = username2;
			myWhiteMage2.mHealth = deadHealth;
			myWhiteMage2.mSpecialtySpell = spell2;
			myWhiteMage2.add_datum(auxiliaryKey2)->push_string_back(nameOfPet2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), deadHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell2);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey2)->get_string_at(0), nameOfPet2);

			WhiteMage myWhiteMage1Copy = myWhiteMage;
			myWhiteMage2 = std::move(myWhiteMage);
			Assert::AreEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage1Copy[1].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[2].get_int_at(0), myWhiteMage1Copy[2].get_int_at(0));
			Assert::AreEqual(myWhiteMage2[3].get_string_at(2), myWhiteMage1Copy[3].get_string_at(2));
			Assert::AreEqual(myWhiteMage2[4].get_string_at(0), myWhiteMage1Copy[4].get_string_at(0));
			Assert::AreEqual(myWhiteMage2[5].get_string_at(0), myWhiteMage1Copy[5].get_string_at(0));
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2.mHealth);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey)->get_string_at(0), nameOfHat1);
			Assert::AreEqual(myWhiteMage2.get_datum_by_key(auxiliaryKey2), (Datum*)nullptr);

			myWhiteMage2 = std::move(myWhiteMage2); // Intentional warning: use of a moved object
			Assert::AreEqual(myWhiteMage2[1].get_string_at(0), myWhiteMage1Copy[1].get_string_at(0));
		}

		TEST_METHOD(CloneTest)
		{
			WhiteMage myWhiteMage;
			std::string username1 = "iobgtanuki";
			myWhiteMage.mName = username1;
			Assert::AreEqual(myWhiteMage.mName, username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);

			std::string spell1 = "Heavenly Blessing";
			myWhiteMage.mSpecialtySpell = spell1;
			Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell1);
			Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);

			Scope* scopePtr = myWhiteMage.Clone();
			WhiteMage* myWhiteMage2 = (WhiteMage*)scopePtr;

			Assert::AreEqual(myWhiteMage2->mName, username1);
			Assert::AreEqual(myWhiteMage2->get_datum_by_key("mName")->get_string_at(0), username1);
			Assert::AreEqual(myWhiteMage2->get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage2->mHealth);
			Assert::AreEqual(myWhiteMage2->mSpecialtySpell, spell1);
			Assert::AreEqual(myWhiteMage2->get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);

			delete scopePtr;
		}

		TEST_METHOD(DeepInheritanceTest)
		{
			WhiteMage myWhiteMage;

			Assert::IsTrue(myWhiteMage.is_prescribed_attribute("mName"));
			Assert::IsTrue(myWhiteMage.is_prescribed_attribute("mHealth"));
			Assert::IsTrue(myWhiteMage.is_prescribed_attribute("mSpells"));
			Assert::IsTrue(myWhiteMage.is_prescribed_attribute("mSpecialtySpell"));
		}

		TEST_METHOD(TwoWayBindingAfterCopyTest)
		{
			{
				WhiteMage myWhiteMage;
				std::string username1 = "iobgtanuki";
				std::string username2 = "bloonz";
				myWhiteMage.mName = username1;
				Assert::AreEqual(myWhiteMage.mName, username1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username1);
				myWhiteMage.get_datum_by_key("mName")->set_string_at(0, username2);
				Assert::AreEqual(myWhiteMage.mName, username2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username2);

				--myWhiteMage.mHealth;
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);
				myWhiteMage.get_datum_by_key("mHealth")->set_int_at(0, myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0) - 1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mHealth")->get_int_at(0), myWhiteMage.mHealth);

				std::string spell1 = "Heavenly Blessing";
				std::string spell2 = "Wrath of God";
				myWhiteMage.mSpecialtySpell = spell1;
				Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell1);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
				myWhiteMage.get_datum_by_key("mSpecialtySpell")->set_string_at(0, spell2);
				Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell2);

				// Copy and test 2 way binding
				WhiteMage myWhiteMage2(myWhiteMage);
				myWhiteMage2.mName = username1;
				Assert::AreEqual(myWhiteMage.mName, username2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mName")->get_string_at(0), username2);
				Assert::AreEqual(myWhiteMage2.mName, username1);
				Assert::AreEqual(myWhiteMage2.get_datum_by_key("mName")->get_string_at(0), username1);

				myWhiteMage2.mSpecialtySpell = spell1;
				Assert::AreEqual(myWhiteMage.mSpecialtySpell, spell2);
				Assert::AreEqual(myWhiteMage.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell2);
				Assert::AreEqual(myWhiteMage2.mSpecialtySpell, spell1);
				Assert::AreEqual(myWhiteMage2.get_datum_by_key("mSpecialtySpell")->get_string_at(0), spell1);
			}
		}


	private:
		// ported from FooTests.cpp
		inline static _CrtMemState _startMemState;
	};
}
