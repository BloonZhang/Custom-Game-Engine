#pragma once

#include "Definitions.h"
#include "Action.h"
#include "IFactory.h"

namespace Fiea::GameEngine::test
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);


	public:
		ActionIncrement();

		// Probably unused because these would be set through JSON.
		// Used for testing purposes
		void set_name_of_target_datum(const std::string&);
		void set_increment_amount(int);

		// Inherited from Action
		virtual void Update(const GameTime&) override;

		// Inherited from Attributed
		virtual ActionIncrement* Clone() const override;
		static std::vector<Signature> get_signatures();

	private:
		std::string mNameOfTargetDatum;
		int mIncrementAmount;
	};

	MAKE_CONCRETE_FACTORY(ActionIncrement);
}