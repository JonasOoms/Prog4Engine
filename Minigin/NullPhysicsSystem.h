#pragma once
#include "PhysicsSystem.h"
class NullPhysicsSystem : public PhysicsSystem
{
public:
	~NullPhysicsSystem() = default;
	void PhysicsUpdate(float) override {};
	void RegisterPhysicsComponent(PhysicsComponent*) override {};
	void UnregisterPhysicsComponent(PhysicsComponent*) override {};
	std::optional<HitInfo> Raycast(const glm::vec2&, const glm::vec2&, float) override { return std::nullopt; };
	std::optional<HitInfo> Raycast(const glm::vec2&, const glm::vec2&, float, PhysicsComponent*) override { return std::nullopt; };
};

