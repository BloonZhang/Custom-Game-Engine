#pragma once

#include "Definitions.h"
#include "Action.h"

namespace Fiea::GameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		ActionList();
		Datum* get_list_of_actions_datum();
		void add_action_to_list(Action*);

		// Inherited from Action

		virtual void Update(const GameTime&) override;

		// Inherited from Attributed
	protected:
		ActionList(size_t);
	public:
		virtual ActionList* Clone() const override = 0;
		static std::vector<Signature> get_signatures();
	private:

	};
}