#pragma once
#include "PhysicsComponent.h"
#include "GameObject.h"

class BounceyPhysicsComponent final : public PhysicsComponent
{
public:
	BounceyPhysicsComponent(glm::vec2 size, glm::vec2 velocity, dae::GameObject* tankFiredFrom);

	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
	virtual void Update(float deltaTime) override;
private:
	dae::GameObject* m_TankFiredFrom;
	int m_countCollisions{};
};

