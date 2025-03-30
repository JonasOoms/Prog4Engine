#include "PlayerControllerComponent.h"
#include "GameObject.h"

PlayerControllerComponent::PlayerControllerComponent(float speed = 1000.f):
	m_Speed{speed}
{
}

void PlayerControllerComponent::Update(float elapsedSec)
{
	if (m_IsDirty)
	{
		auto originalPos = GetOwner()->GetTransform().GetPosition();
		auto normalizedMovement = glm::normalize(m_MovementInput);
		
		if (std::isnan(normalizedMovement.x) || std::isnan(normalizedMovement.y))
		{
			normalizedMovement = glm::vec2(0, 0);
		}

		glm::vec2 total = glm::vec2(originalPos.x, originalPos.y) + normalizedMovement * m_Speed * elapsedSec;
		GetOwner()->SetPosition(total.x, total.y);
		m_MovementInput = glm::vec2(0, 0);
		m_IsDirty = false;
	}
}

void PlayerControllerComponent::AddMovementInput(const glm::vec2& movementInput)
{
	m_MovementInput += movementInput;
	m_IsDirty = true;
}
