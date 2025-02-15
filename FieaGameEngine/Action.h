#pragma once

#include "Definitions.h"
#include "IFactory.h"
#include "Attributed.h"

namespace Fiea::GameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		Action();
		~Action() = default;
		Action(const Action&) = default;
		Action(Action&&) = default;
		Action& operator=(const Action&) = default;
		Action& operator=(Action&&) = default;

	public:
		const std::string name() const;
		void setName(const std::string&);

		virtual void Update(const GameTime&) = 0;

		// Inherited from Attributed
		//virtual Action* Clone() const override = 0;
		static std::vector<Signature> get_signatures();
	protected:
		Action(size_t);
	private:
		std::string mName;
	};

	//MAKE_CONCRETE_FACTORY(Action);
}
