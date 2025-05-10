#include "BounceyPhysicsComponent.h"
#include "PhysicsSystem.h"
#include <memory>


BounceyPhysicsComponent::BounceyPhysicsComponent(glm::vec2 size, glm::vec2 velocity, dae::GameObject* tankFiredFrom):
	PhysicsComponent(size),
	m_TankFiredFrom{tankFiredFrom},
	m_State{std::make_unique<FlyingState>()}
{
	SetVelocity(velocity);
}

void BounceyPhysicsComponent::OnCollide(float dt , PhysicsComponent& component,dae::GameObject& gameobject, const HitInfo& hitInfo)
{
	m_State->OnCollision(*this, dt, component, gameobject, hitInfo);
}

void BounceyPhysicsComponent::Update(float deltaTime)
{
	m_State->Update(*this, deltaTime);
}

void BounceyPhysicsComponent::SetState(BulletState* state)
{
	m_State = std::unique_ptr<BulletState>{ state };
}

void FlyingState::Update(BounceyPhysicsComponent& bullet, float dt)
{
	glm::vec2 pos = bullet.GetOwner()->GetPosition();
	glm::vec2 delta = bullet.GetVelocity() * dt;
	bullet.GetOwner()->SetPosition(pos.x + delta.x, pos.y + delta.y);
	m_HasBounced = false;
}

void FlyingState::OnCollision(BounceyPhysicsComponent& bullet, float dt, PhysicsComponent& component, dae::GameObject& gameObject, const HitInfo& hitInfo)
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
					bullet.SetState(new ExplodingState());
				}
			}
		}
	}
}

void ExplodingState::Update(BounceyPhysicsComponent& bullet, float dt)
{
	m_ExplosionCountdown -= dt;
	if (m_ExplosionCountdown <= 0)
	{
		bullet.GetOwner()->Destroy();
	}
}
