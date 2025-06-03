#pragma once

#include <glm.hpp>
#include "Command.h"
#include "GameObject.h"


class SimpleMoveCommand final : public Command
{
public:
	SimpleMoveCommand(dae::GameObject* pGameObject, const glm::vec2& movement, float speed);
	~SimpleMoveCommand() = default;

	void SetMovementVector(const glm::vec2& movement);

	// Inherited via Command
	void Execute() override;

private:
	dae::GameObject* m_pGameObject{};
	glm::vec2 m_MovementVector;
	float m_Speed;
};