#pragma once
#include <any>

using event_type = unsigned int;

namespace EventType
{
	inline constexpr unsigned int sdbm_hash(const char* str) {
		unsigned int hash = 0;
		int c;
		while ((c = *str++)) {
			hash = c + (hash << 6) + (hash << 16) - hash;
		}
		return hash;
	}


}

namespace EngineEvents
{
	constexpr event_type EVENT_VALUE_CHANGED = EventType::sdbm_hash("EVENT_VALUE_CHANGED");
}

struct Event
{
	event_type eventType;
	std::any eventContext;
};

namespace EventContext
{
	template<typename T>
	struct ValueChangedEventContext
	{
		T newValue;
	};
}
