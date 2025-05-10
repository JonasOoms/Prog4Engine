#pragma once
#include "PhysicsComponent.h"

class StaticPhysicsComponent : public PhysicsComponent
{

public:
	StaticPhysicsComponent(const glm::vec2& size)
		: PhysicsComponent(size)
	{
	};
	virtual void OnCollide(float ,PhysicsComponent&, dae::GameObject&, const HitInfo&) override {};
};

