#include "ShootCommand.h"
#include <chrono>
#include "GamePrefabs.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"
#include "LinearBulletComponent.h"
#include "VelocityBasedSpriteRenderer.h"

PlayerShootCommand::PlayerShootCommand(TurretComponent* component, dae::Scene& scene , float speed, float CooldownTime):
	m_Component{component},
	m_Speed{speed},
	m_CoolDownTime{CooldownTime},
	m_Scene{scene}
{
}

void PlayerShootCommand::Execute()
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
		ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("SFX1"), 5.f);
		
	}

}

EnemyShootCommand::EnemyShootCommand(dae::GameObject* enemy, float speed):
	m_Enemy{enemy},
	m_Speed{ speed }
{
}

void EnemyShootCommand::Execute()
{

	constexpr glm::vec2 size{ 15.f,15.f };
	dae::Scene& scene = dae::SceneManager::GetInstance().GetCurrentScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<LinearBulletComponent>(size, m_Speed, m_Direction);
	
	go->AddComponent<VelocityBasedSpriteRenderer>(size.x, size.y)->SetTexture("Textures/T_EnemyProjectile.png");
	

	glm::vec2 middle = m_Enemy->GetComponent<PhysicsComponent>()->GetPosition();

	go->SetPosition(middle.x, middle.y);
	scene.Add(std::move(go));

}
