#include "BounceyPhysicsComponent.h"
#include "PhysicsSystem.h"
#include "StaticPhysicsComponent.h"


BounceyPhysicsComponent::BounceyPhysicsComponent(glm::vec2 size, glm::vec2 velocity, dae::GameObject* tankFiredFrom):
	PhysicsComponent(size),
	m_TankFiredFrom{tankFiredFrom}
{
	m_Velocity = velocity;
}

void BounceyPhysicsComponent::OnCollide(float dt , PhysicsComponent& component,dae::GameObject& gameobject, const HitInfo& hitInfo)
{
	if (&gameobject == m_TankFiredFrom)
	{
		if (m_countCollisions < 5)
		{
			PhysicsComponent::OnCollide(dt, component, gameobject, hitInfo);
			m_Velocity = glm::reflect(m_Velocity, hitInfo.normal);
			++m_countCollisions;
		}
		else
		{
			GetOwner()->Destroy();
		}
	}
}

void BounceyPhysicsComponent::Update(float deltaTime)
{
	glm::vec2 pos = GetOwner()->GetPosition();
	glm::vec2 delta =  m_Velocity * deltaTime;
	GetOwner()->SetPosition(pos.x + delta.x, pos.y + delta.y);
}