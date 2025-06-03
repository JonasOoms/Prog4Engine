#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "GameEvents.h"

PlayerControllerComponent::PlayerControllerComponent(float speed = 1000.f):
	m_Speed{speed},
	m_MovedEvent{ std::make_unique<EventDispatcher>() }
{
}

void PlayerControllerComponent::Update(float elapsedSec)
{
	if (m_IsDirty)
	{
		auto originalPos = GetOwner()->GetTransform().GetWorldPosition();
		auto normalizedMovement = glm::normalize(m_MovementInput);
		
		if (std::isnan(normalizedMovement.x) || std::isnan(normalizedMovement.y))
		{
			normalizedMovement = glm::vec2(0, 0);
		}
		glm::vec2 total = glm::vec2(originalPos.x, originalPos.y) + normalizedMovement * m_Speed * elapsedSec;
		GetOwner()->SetPosition(total.x, total.y);
		TriggerMovementEvents(normalizedMovement, total);
		m_MovementInput = glm::vec2(0, 0);
		m_IsDirty = false;
	}
}

void PlayerControllerComponent::SetMovementInput(const glm::vec2& movementInput)
{
	m_MovementInput = movementInput;
	m_IsDirty = true;
}

void PlayerControllerComponent::AddMovementInput(const glm::vec2& movementInput)
{
	m_MovementInput += movementInput;
	m_IsDirty = true;
}

void PlayerControllerComponent::TriggerMovementEvents(const glm::vec2& movementDelta, const glm::vec2& newPosition)
{
	Event playerMoveEvent;
	playerMoveEvent.eventType = TRONEvents::EVENT_PLAYER_MOVE;
	TRONEvents::EventContexts::PlayerMoveEventContext playerMoveContext{movementDelta, newPosition};
	playerMoveEvent.eventContext = playerMoveContext;
	m_MovedEvent->NotifyObservers(playerMoveEvent);
}
