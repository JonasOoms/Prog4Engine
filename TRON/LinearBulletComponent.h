#pragma once
#include <PhysicsComponent.h>
class LinearBulletComponent : public PhysicsComponent
{
public:
	LinearBulletComponent(const glm::vec2& size, float speed, const glm::vec2& direction);

	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
};

