#include "LinearBulletComponent.h"
#include "PlayerHandlerComponent.h"

LinearBulletComponent::LinearBulletComponent(const glm::vec2& size, float speed, const glm::vec2& direction):
	PhysicsComponent(size)
{
	SetVelocity(glm::normalize(direction)* speed);
}

void LinearBulletComponent::OnCollide(float , PhysicsComponent& otherComp, dae::GameObject& other, const HitInfo&)
{
	if (otherComp.GetIsStatic())
	{
		GetOwner()->Destroy();
	}
	if (PlayerTankHandlerComponent* handler = other.GetComponent<PlayerTankHandlerComponent>())
	{
		handler->Hurt();
		GetOwner()->Destroy();
	}

}
