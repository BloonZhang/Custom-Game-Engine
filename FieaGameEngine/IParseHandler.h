#pragma once

#include "Definitions.h"
#include "Wrapper.h"
#include "json/json.h"

namespace Fiea::GameEngine
{
	class IParseHandler
	{
	public:
		virtual ~IParseHandler() = default;
		virtual void initialize_handler();
		virtual bool start_handler(Wrapper* wrapper, JsonPair jsonPair) = 0;
		virtual bool end_handler(Wrapper* wrapper, JsonPair jsonPair) = 0;
		virtual void cleanup_handler();

		bool is_active() const;

	protected:
		IParseHandler() = default;

	private:
		bool mIsActive = false;
	};
}