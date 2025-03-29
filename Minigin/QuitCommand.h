#pragma once
#include "Command.h"

class QuitCommand final : public Command
{
public:
	virtual void Execute() override;
};

