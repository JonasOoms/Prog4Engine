#pragma once
#include <any>

namespace EventType
{
	inline unsigned int sdbm_hash(const char* str) {
		unsigned int hash = 0;
		int c;
		while ((c = *str++)) {
			hash = c + (hash << 6) + (hash << 16) - hash;
		}
		return hash;
	}
}

struct Event
{
	unsigned int eventType;
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
