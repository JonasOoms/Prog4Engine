#include "PhysicsComponent.h"
#include "Transform.h"
#include "ServiceLocator.h"

PhysicsComponent::PhysicsComponent(const glm::vec2& size):
	Component()
{
	m_Bounds = size;
	// Register with the physics system
}

PhysicsComponent::~PhysicsComponent()
{
	// Unregister from the physics system
	ServiceLocator::GetPhysicsSystem()->UnregisterPhysicsComponent(this);
}

void PhysicsComponent::BeginPlay()
{
	ServiceLocator::GetPhysicsSystem()->RegisterPhysicsComponent(this);
}

void PhysicsComponent::EndPlay()
{
	ServiceLocator::GetPhysicsSystem()->UnregisterPhysicsComponent(this);
}

void PhysicsComponent::ComponentOwnerInitialized()
{
	// Register with the physics system
	ServiceLocator::GetPhysicsSystem()->RegisterPhysicsComponent(this);
}

void PhysicsComponent::OnCollide(float, PhysicsComponent& , dae::GameObject&, const HitInfo& )
{ 
}

Engine::Rect PhysicsComponent::GetBoundingBox() const
{
	dae::Transform& transform = GetOwner()->GetTransform();
	glm::vec2 position = transform.GetWorldPosition();
	return Engine::Rect{ position.x , position.y, m_Bounds.x, m_Bounds.y };
}

glm::vec2 PhysicsComponent::GetPosition() const
{
	return GetOwner()->GetPosition() + glm::vec2(m_Bounds.x * 0.5f, m_Bounds.y * 0.5f);
}

glm::vec2 PhysicsComponent::GetMin() const
{
    return GetPosition() - m_Bounds * 0.5f;  
}

glm::vec2 PhysicsComponent::GetMax() const
{
    return GetPosition() + m_Bounds * 0.5f;  
}