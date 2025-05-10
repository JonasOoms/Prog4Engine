#pragma once
#include <any>
#include "Transform.h"

using event_type = unsigned int;

namespace EventType
{
	inline consteval unsigned int sdbm_hash(const char* str) {
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
	constexpr event_type EVENT_GAMEOBJECT_TRANSFORMCHANGED = EventType::sdbm_hash("EVENT_GAMEOBJECT_TRANSFORMCHANGED");
	constexpr event_type EVENT_GAMEOBJECT_CHILDADDED = EventType::sdbm_hash("EVENT_GAMEOBJECT_CHILDADDED");
	constexpr event_type EVENT_GAMEOBJECT_CHILDREMOVED = EventType::sdbm_hash("EVENT_GAMEOBJECT_CHILDREMOVED");
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

	struct GameObjectTransformChangedEventContext
	{
		dae::GameObject* gameObject;
		dae::Transform oldTransform;
		dae::Transform newTransform;
	};

	struct GameObjectChildHierarchyChangedContext
	{
		dae::GameObject* parent;
		dae::GameObject* child;
	};
}
