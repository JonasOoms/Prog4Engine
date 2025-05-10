#pragma once
#include "Command.h"
#include "TurretComponent.h"

class TurretAngleChangeCommand : public Command
{
public:

	TurretAngleChangeCommand(TurretComponent* turret, float angle);

	// Inherited via Command
	void Execute() override;
private:
	TurretComponent* m_Turret;
	float m_Angle;
};