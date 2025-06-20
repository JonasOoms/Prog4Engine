#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "GameEvents.h"
#include "Minigin.h"
#include "PhysicsComponent.h"
#include <algorithm>

PlayerControllerComponent::PlayerControllerComponent(float speed = 1000.f):
	m_Speed{speed},
	m_MovedEvent{ std::make_unique<EventDispatcher>() }
{
}

void PlayerControllerComponent::ComponentOwnerInitialized()
{
	m_CachedPhysicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
}

void PlayerControllerComponent::Update(float dt)
{
  
        auto originalPos = GetOwner()->GetTransform().GetWorldPosition();
        auto normalizedMovement = glm::normalize(m_MovementInput);

        if (std::isnan(normalizedMovement.x)) normalizedMovement.x = 0;
		if (std::isnan(normalizedMovement.y)) normalizedMovement.y = 0;
        
		if (m_CachedPhysicsComponent)
		{
			glm::vec2 velocity = normalizedMovement * m_Speed;
			m_CachedPhysicsComponent->SetVelocity(velocity);
			if (velocity.length() > 0.f)
			{
				TriggerMovementEvents(normalizedMovement, GetOwner()->GetPosition() + normalizedMovement * m_Speed * dt);
			}
		}

        m_MovementInput = glm::vec2(0, 0);
        m_IsDirty = false;
   
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
