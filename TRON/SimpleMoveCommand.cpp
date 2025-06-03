#include "SimpleMoveCommand.h"
#include "Minigin.h"

SimpleMoveCommand::SimpleMoveCommand(dae::GameObject* pGameObject, const glm::vec2& movement, float speed):
	m_pGameObject{pGameObject},
	m_MovementVector{movement},
	m_Speed{speed}
{
}

void SimpleMoveCommand::SetMovementVector(const glm::vec2& movement)
{
	m_MovementVector = movement;
}

void SimpleMoveCommand::Execute()
{
	const glm::vec2 position = m_pGameObject->GetPosition();
	glm::vec2 newPosition = position + m_MovementVector * m_Speed * dae::Minigin::GetDeltaTime();
	m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}
