#pragma once
#include "Event.h"
#include "glm.hpp"
#include "IEnemy.h"

namespace TRONEvents
{
	namespace EventContexts
	{
		struct PlayerMoveEventContext
		{
			glm::vec2 movementDelta;
			glm::vec2 newPosition;
		};

		struct EnemyDeathEventContext
		{
			dae::GameObject* enemy;
		};

	}
	constexpr event_type EVENT_PLAYER_MOVE = EventType::sdbm_hash("EVENT_PLAYER_MOVE");
	constexpr event_type EVENT_ENEMY_DEATH = EventType::sdbm_hash("EVENT_ENEMY_DEATH");
	constexpr event_type EVENT_PLAYER_DEATH = EventType::sdbm_hash("EVENT_PLAYER_DEATH");
}
