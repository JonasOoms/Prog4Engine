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

void PhysicsComponent::ComponentOwnerInitialized()
{
	// Register with the physics system
	ServiceLocator::GetPhysicsSystem()->RegisterPhysicsComponent(this);
}

void PhysicsComponent::OnCollide(float , const PhysicsComponent&, const dae::GameObject&, const HitInfo& hitInfo)
{
	auto currentPosition = GetOwner()->GetPosition();
	auto collisionCorrection = glm::vec2(currentPosition.x,currentPosition.y) + (hitInfo.normal * hitInfo.penetrationDepth);
	GetOwner()->SetPosition(collisionCorrection.x, collisionCorrection.y);
}

Engine::Rect PhysicsComponent::GetBoundingBox() const
{
	dae::Transform& transform = GetOwner()->GetTransform();
	glm::vec2 position = transform.GetWorldPosition();
	return Engine::Rect{ position.x , position.y, m_Bounds.x, m_Bounds.y };
}

glm::vec2 PhysicsComponent::GetPosition() const
{
	Engine::Rect boundingBox = GetBoundingBox();
	return (glm::vec2(boundingBox.x + boundingBox.width * 0.5f, boundingBox.y + boundingBox.height * 0.5f));
}
