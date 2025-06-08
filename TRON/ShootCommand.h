#pragma once
#include "Command.h"
#include "TurretComponent.h"
#include "Scene.h"

class PlayerShootCommand : public Command
{
public:
	PlayerShootCommand(TurretComponent* component, dae::Scene& scene, float speed, float coolDownTime);
	// Inherited via Command
	void Execute() override;
private:
	dae::Scene& m_Scene;
	TurretComponent* m_Component;
	const float m_Speed;
	const float m_CoolDownTime;
	float m_LastUsedTime{};
};

class EnemyShootCommand : public Command
{
public:

	EnemyShootCommand(dae::GameObject* enemy, float speed);
	void SetDirection(const glm::vec2& direction) { m_Direction = direction; }

	// Inherited via Command
	void Execute() override;

private:
	dae::GameObject* m_Enemy{};
	glm::vec2 m_Direction{};
	float m_Speed;

};