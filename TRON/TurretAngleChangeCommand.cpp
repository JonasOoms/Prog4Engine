#include "TurretAngleChangeCommand.h"

TurretAngleChangeCommand::TurretAngleChangeCommand(TurretComponent* turret, float angle):
	m_Turret{turret},
	m_Angle{angle}
{
}

void TurretAngleChangeCommand::Execute()
{
	if (m_Turret)
	{
		m_Turret->AddAngle(m_Angle);
	}
}
