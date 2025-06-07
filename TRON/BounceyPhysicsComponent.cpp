#include "BounceyPhysicsComponent.h"
#include "PhysicsSystem.h"
#include "IEnemy.h"
#include <memory>
#include "Renderer.h"


BounceyPhysicsComponent::BounceyPhysicsComponent(const glm::vec2& size, const glm::vec2& velocity, dae::GameObject* tankFiredFrom):
	PhysicsComponent(size),
	m_TankFiredFrom{tankFiredFrom},
	m_State{std::make_unique<FlyingState>()}
{
	SetVelocity(velocity);
}

void BounceyPhysicsComponent::OnCollide(float dt , PhysicsComponent& component,dae::GameObject& gameobject, const HitInfo& hitInfo)
{
	if (auto newState = m_State->OnCollision(*this, dt, component, gameobject, hitInfo))
	{
		m_State = std::move(newState);
	}
}

void BounceyPhysicsComponent::Update(float deltaTime)
{
	if (auto newState = m_State->Update(*this, deltaTime)) {
		m_State = std::move(newState);
	}
}

void BounceyPhysicsComponent::DebugDraw()
{
	m_State->DebugDraw();
}


std::unique_ptr<BulletState> FlyingState::Update(BounceyPhysicsComponent& , float )
{
	m_HasBounced = false;
	return nullptr;
}

std::unique_ptr<BulletState> FlyingState::OnCollision(BounceyPhysicsComponent& bullet, float , PhysicsComponent& component, dae::GameObject& gameObject, const HitInfo& hitInfo)
{
	if (component.GetIsStatic())
	{
		if (!m_HasBounced)
		{
			if (!(&gameObject == bullet.GetTankFiredFrom()))
			{
				if (m_countCollisions < 5)
				{
					bullet.SetVelocity(glm::reflect(bullet.GetVelocity(), hitInfo.normal));
					m_HasBounced = true;
					++m_countCollisions;
					hits.emplace_back(hitInfo);
				}
				else
				{
					return std::make_unique<ExplodingState>();
				}
			}
		}
	}
	if (IEnemyHandler* enemy = gameObject.GetComponent<IEnemyHandler>())
	{
		bullet.GetOwner()->Destroy();
		enemy->Hurt(1);
	}
	return nullptr;
}

void FlyingState::DebugDraw()
{
	for (HitInfo& hitInfo : hits)
	{
		dae::Renderer::GetInstance().drawLine(int(hitInfo.hitPosition.x), int(hitInfo.hitPosition.y), int(hitInfo.hitPosition.x + hitInfo.normal.x*10), int(hitInfo.hitPosition.y + hitInfo.normal.y*10), SDL_Color{ 255,0,255,255 });
	}
}

std::unique_ptr<BulletState> ExplodingState::Update(BounceyPhysicsComponent& bullet, float dt)
{
	m_ExplosionCountdown -= dt;
	if (m_ExplosionCountdown <= 0)
	{
		bullet.GetOwner()->Destroy();
	}
	return nullptr;
}
