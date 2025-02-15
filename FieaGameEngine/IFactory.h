#pragma once

#include "Definitions.h"

namespace Fiea::GameEngine
{
	class IFactory
	{
	public:
		[[nodiscard]] virtual Scope* create() const = 0;
		virtual const std::string get_class_name() const = 0;
	};

	// Macro for creating concrete factories
#define MAKE_CONCRETE_FACTORY(_Concrete) \
	class _Concrete##Factory : public IFactory \
	{\
		inline virtual Scope* create() const override { return new _Concrete(); }\
		inline virtual const std::string get_class_name() const override { return #_Concrete; }\
	};
}
