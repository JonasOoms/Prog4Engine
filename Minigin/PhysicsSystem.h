#pragma once
#include "PhysicsComponent.h"
#include <optional>

struct HitInfo
{
	glm::vec2 hitPosition{};
	glm::vec2 normal{};
	float penetrationDepth{};
	PhysicsComponent* pHitComponent{};
};

class PhysicsSystem
{
public:
	virtual ~PhysicsSystem() = default;
	virtual void PhysicsUpdate(float deltaTime) = 0;
	virtual void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) = 0;
	virtual void UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) = 0;
	virtual void DebugDraw() {};

	// Physics Utility
	virtual std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance = 1000.f) = 0;
	virtual std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore) = 0;
};

