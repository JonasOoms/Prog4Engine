#pragma once
#include "Command.h"
#include "TurretComponent.h"
#include "Scene.h"

class ShootCommand : public Command
{
public:
	ShootCommand(TurretComponent* component, dae::Scene& scene, float speed, float coolDownTime);
	// Inherited via Command
	void Execute() override;
private:
	dae::Scene& m_Scene;
	TurretComponent* m_Component;
	const float m_Speed;
	const float m_CoolDownTime;
	float m_LastUsedTime{};
};

