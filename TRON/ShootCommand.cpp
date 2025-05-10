#include "ShootCommand.h"
#include <chrono>
#include "GamePrefabs.h"

ShootCommand::ShootCommand(TurretComponent* component, dae::Scene& scene , float speed, float CooldownTime):
	m_Component{component},
	m_Speed{speed},
	m_CoolDownTime{CooldownTime},
	m_Scene{scene}
{
}

void ShootCommand::Execute()
{
	auto now = std::chrono::high_resolution_clock::now();
	float currentTime = std::chrono::duration<float>(now.time_since_epoch()).count();
	if ((currentTime - m_CoolDownTime) > 0)
	{
		m_LastUsedTime = currentTime;
		float angle = m_Component->GetAngle();
		glm::vec2 velocity = glm::vec2{ cosf(angle) * m_Speed, -sinf(angle) * m_Speed };
		TRONGameObjects::PrefabFactory{}.CreateProjectile(m_Scene, m_Component->GetOwner(), velocity);
	}

}
