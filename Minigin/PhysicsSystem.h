#pragma once
#include "PhysicsComponent.h"

struct HitInfo
{
	glm::vec2 hitPosition;
	glm::vec2 normal;
	float penetrationDepth;
};

class PhysicsSystem
{
public:
	virtual ~PhysicsSystem() = default;
	virtual void PhysicsUpdate(float deltaTime) = 0;
	virtual void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) = 0;
	virtual void UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) = 0;
	virtual void DebugDraw() {};
};

