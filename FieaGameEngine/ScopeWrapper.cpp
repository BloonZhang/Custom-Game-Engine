#include "pch.h"
#include "ScopeWrapper.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(ScopeWrapper);

ScopeWrapper::ScopeWrapper(Scope* scopePtr) : externalScope(scopePtr), currentScope(scopePtr)
{
}

Scope* ScopeWrapper::get_root_scope() const
{
	return externalScope;
}

Scope* ScopeWrapper::get_current_scope() const
{
	return currentScope;
}

void ScopeWrapper::set_current_scope(Scope* scopePtr)
{
	currentScope = scopePtr;
}
