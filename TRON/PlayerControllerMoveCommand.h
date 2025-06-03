#pragma once
#include "PlayerControllerCommand.h"

class PlayerControllerMoveCommand : public PlayerControllerCommand
{
public:
	PlayerControllerMoveCommand(PlayerControllerComponent* playerControllerLink, const glm::vec2& movementInput) :
		PlayerControllerCommand(playerControllerLink),
		m_MovementInput{ movementInput }
	{
	};

	~PlayerControllerMoveCommand() = default;

	virtual void Execute() override
	{
		m_pPlayerControllerLink->SetMovementInput(m_MovementInput);
	}
private:
	glm::vec2 m_MovementInput;
};

