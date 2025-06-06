#include "BounceyPhysicsComponent.h"
#include "PhysicsSystem.h"
#include "IEnemy.h"
#include <memory>


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


std::unique_ptr<BulletState> FlyingState::Update(BounceyPhysicsComponent& bullet, float dt)
{
	glm::vec2 pos = bullet.GetOwner()->GetPosition();
	glm::vec2 delta = bullet.GetVelocity() * dt;
	bullet.GetOwner()->SetPosition(pos.x + delta.x, pos.y + delta.y);
	m_HasBounced = false;
	return nullptr;
}

std::unique_ptr<BulletState> FlyingState::OnCollision(BounceyPhysicsComponent& bullet, float dt, PhysicsComponent& component, dae::GameObject& gameObject, const HitInfo& hitInfo)
{
	if (component.GetIsStatic())
	{
		if (!m_HasBounced)
		{
			if (!(&gameObject == bullet.GetTankFiredFrom()))
			{
				if (m_countCollisions < 5)
				{
					bullet.PhysicsComponent::OnCollide(dt, component, gameObject, hitInfo);
					bullet.SetVelocity(glm::reflect(bullet.GetVelocity(), hitInfo.normal));
					m_HasBounced = true;
					++m_countCollisions;
				}
				else
				{
					return std::make_unique<ExplodingState>();
				}
			}
		}
	}
	if (IEnemyHandlerComponent* enemy = gameObject.GetComponent<IEnemyHandlerComponent>())
	{
		bullet.GetOwner()->Destroy();
		enemy->Hurt(1);
	}
	return nullptr;
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
