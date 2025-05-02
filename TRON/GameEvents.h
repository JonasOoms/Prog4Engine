#pragma once
#include "Event.h"
#include "glm.hpp"

namespace TRONEvents
{
	namespace EventContexts
	{
		struct PlayerMoveEventContext
		{
			glm::vec2 movementDelta;
			glm::vec2 newPosition;
		};
	}
	constexpr event_type EVENT_PLAYER_MOVE = EventType::sdbm_hash("EVENT_PLAYER_MOVE");
}
