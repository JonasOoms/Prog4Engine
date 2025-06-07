#include "ShootCommand.h"
#include <chrono>
#include "GamePrefabs.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"

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
	if ((currentTime - m_LastUsedTime) >= m_CoolDownTime)
	{
		m_LastUsedTime = currentTime;
		float angleDegrees = m_Component->GetAngle();
		float angle = glm::radians(angleDegrees); 

		glm::vec2 velocity{ cosf(angle), sinf(angle) };
		velocity = glm::normalize(velocity) * m_Speed;
		TRONGameObjects::PrefabFactory{}.CreateProjectile(m_Scene, m_Component->GetOwner()->GetParent(), velocity * m_Speed);
		ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("SFX1"), 128.f);
		
	}

}
