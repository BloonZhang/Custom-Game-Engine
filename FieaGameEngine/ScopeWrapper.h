#pragma once

#include "Definitions.h"
#include "RTTI.h"
#include "Wrapper.h"

namespace Fiea::GameEngine
{
	class ScopeWrapper : public Wrapper
	{
		RTTI_DECLARATIONS(ScopeWrapper, Wrapper);

	public:
		ScopeWrapper(Scope*);
		Scope* get_root_scope() const;
		Scope* get_current_scope() const;
		void set_current_scope(Scope*);

	private:
		ScopeWrapper() = delete;
		Scope* externalScope;
		Scope* currentScope;
	};
}