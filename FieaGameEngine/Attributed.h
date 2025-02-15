#pragma once

#include "Scope.h"
#include "RTTI.h"
#include "Definitions.h"
#include "TypeRegistrySingleton.hpp"

namespace Fiea::GameEngine
{

	class Attributed : public Scope
	{
        RTTI_DECLARATIONS(Attributed, Scope);

    protected:
        Attributed(size_t);
        size_t mTypeId;

        virtual ~Attributed() = default;
        Attributed(const Attributed&);
        Attributed(Attributed&&) noexcept;
        Attributed& operator=(const Attributed&);
        Attributed& operator=(Attributed&&);

    public:
        Datum* add_auxiliary_attribute(std::string);

        bool is_attribute(std::string) const;
        bool is_prescribed_attribute(std::string) const;
        bool is_auxiliary_attribute(std::string) const;

        // TODO: make it abstract
        [[nodiscard]] virtual Attributed* Clone() const override = 0;
        // All children classes should also have a static method:
        // static std::vector<Signature> get_signatures();
        // All children classes should also have a constuctor that calls Attributed(size_t)
        // All children classes should also have a constuctor that uses (size_t) for forwarding purposes, unless final

    private:
        Attributed() = delete;
        void add_members_to_scope_tree(IdType);
	};



    /*
    class Child : public Scope
    {
    public:
        Child(int val = 0) : _val(val)
        {
            add_datum("val"s)->SetStorage(&_val, 1);
        }
        Child(const Child& other) : Scope(other), _val(other._val)
        {
            Append("val"s).SetStorage(&_val, 1);
        }
        ~Child() = default;
        Child& operator=(const Child& rhs) {
            if (this == &rhs) return *this;
            Scope::operator=(rhs);
            _val = rhs._val;
            Append("val"s).SetStorage(&_val, 1);
        }
        bool operator==(const Child& rhs) const
        {
            if (Scope::operator!=(rhs))
            {
                return false;
            }
            return _val == rhs._val;
        }
        // scope & rtti overrides
        virtual Child* Clone() const override {
            return new Child(*this);
        }
        virtual std::string ToString() const override { return std::to_string(_val); }
        // virtual bool Equals(const RTTI* other); provided in my RTTI_DECLARATIONS
    private:
        int _val;
    };
    */


}