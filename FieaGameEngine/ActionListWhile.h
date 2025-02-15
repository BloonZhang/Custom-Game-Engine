#pragma once

#include "Definitions.h"
#include "ActionList.h"

namespace Fiea::GameEngine
{
	class ActionListWhile : public ActionList
	{
		RTTI_DECLARATIONS(ActionListWhile, ActionList);

	public:
		ActionListWhile();

		void adopt_action_as_preamble(Action*);
		void adopt_action_as_increment(Action*);

		// Inherited from Action
		virtual void Update(const GameTime&) override;

		// Inherited from Attributed
	protected:
		ActionListWhile(size_t);
	public:
		virtual ActionListWhile* Clone() const override;
		static std::vector<Signature> get_signatures();
	private:
		// Action preamble
		// Action increment
		int mCondition;
	};
}