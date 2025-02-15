#include "pch.h"
#include "CppUnitTest.h"
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::GameEngine::test {
    class Base1 {
    public:
        Base1(float f = 0.f) : _f(f) {}
        virtual ~Base1() {}
        virtual float& GetFloat() { return _f; }
    private:
        float _f;
    };

    class Base2 {
    public:
        Base2(int i = 0) : _i(i) {}
        virtual ~Base2() {}
        virtual int& GetInt() { return _i; }
    private:
        int _i;
    };

    class IBase {
    public:
        virtual ~IBase() {};
        virtual std::string GetClass() const = 0;
    };

    class Child : public Base1, public Base2, public IBase {
    public:
        std::string GetClass() const override { return std::string("Child"); }
    };

    template <class T, class U>
    ptrdiff_t Diff(T* offset, U* base) {
        return reinterpret_cast<ptrdiff_t>(offset) - reinterpret_cast<ptrdiff_t>(base);
    }

    TEST_CLASS(MultipleInheritance) {
        TEST_METHOD(Offsets) {
            constexpr auto zero = ptrdiff_t(0);

            std::stringstream ss;

            Child child;
            Base1* base1 = &child;
            Base2* base2 = &child;
            IBase* ibase = &child;

            // base1 and base2 clearly point to the same memory address, right? WRONG!

            Assert::AreEqual(zero, Diff(base1, &child));
            Assert::AreNotEqual(zero, Diff(base2, &child));
            Assert::AreNotEqual(zero, Diff(ibase, &child));
            Assert::AreNotEqual(zero, Diff(base2, base1));
            Assert::AreNotEqual(zero, Diff(ibase, base2));
            Assert::AreNotEqual(zero, Diff(base1, ibase));

            ss << "sizeof(Child) = " << sizeof(child) << std::endl;
            ss << "offsetof(Base1) = " << Diff(base1, &child) << std::endl;
            ss << "offsetof(_f) = " << Diff(&child.GetFloat(), &child) << std::endl;
            ss << "offsetof(Base2) = " << Diff(base2, &child) << std::endl;
            ss << "offsetof(_i) = " << Diff(&child.GetInt(), &child) << std::endl;
            ss << "offsetof(IBase) = " << Diff(ibase, & child) << std::endl;

            Logger::WriteMessage(ss.str().c_str());

            /*
             * The above highlight the memory layout:
             * 
             * Child
             *     - vtable* for Child implementations for Base1 virtual methods
             *     - Base1 members
             *     - vtable* for Child implementations for Base2 virtual methods
             *     - Base2 members
             *     - vtable* for Child implementations for IBase virtual methods
             *
             * Note that even though child doesn't currently override any virtual methods in Base1 or Base2, the Child class has it's own vtables that 
             * correspond to the methods of each class
             */
        }
    };
}