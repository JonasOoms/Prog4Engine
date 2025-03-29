#pragma once
#include "Command.h"
#include "PlayerControllerComponent.h"

class PlayerControllerCommand : public Command
{
public:
	PlayerControllerCommand(PlayerControllerComponent* playerControllerLink):
		m_pPlayerControllerLink{ playerControllerLink }
	{
	};

	~PlayerControllerCommand() = default;

	virtual void Execute() = 0;

protected:
	PlayerControllerComponent* m_pPlayerControllerLink;
};

