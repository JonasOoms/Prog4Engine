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

void PlayerControllerComponent::Update(float elapsedSec)
{
    if (m_IsDirty)
    {
        auto originalPos = GetOwner()->GetTransform().GetWorldPosition();
        auto normalizedMovement = glm::normalize(m_MovementInput);

        if (std::isnan(normalizedMovement.x)) normalizedMovement.x = 0;
            if (std::isnan(normalizedMovement.y)) normalizedMovement.y = 0;

            // Calculate new position
                glm::vec2 newPos = glm::vec2(originalPos.x, originalPos.y) +
                normalizedMovement * m_Speed * elapsedSec;

            // Get world bounds
                
                const float windowWidth = static_cast<float>(dae::Minigin::windowWidth);
            const float windowHeight = static_cast<float>(dae::Minigin::windowHeight);

            // Get the size of the object (assuming you have access to width/height)
                float objectWidth = GetOwner()->GetComponent<PhysicsComponent>()->GetWidth();
        float objectHeight = GetOwner()->GetComponent<PhysicsComponent>()->GetHeight();

        // Clamp position to world bounds
        newPos.x = std::clamp(newPos.x, 0.0f, windowWidth - objectWidth);
        newPos.y = std::clamp(newPos.y, 0.0f, windowHeight - objectHeight);

        // Set the new position
        GetOwner()->SetPosition(newPos.x, newPos.y);
        TriggerMovementEvents(normalizedMovement, newPos);

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
