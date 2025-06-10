#pragma once
#include <deque>
#include "PhysicsSystem.h"

/// <summary>
/// Keeps track of average time to perform physics update. 
/// </summary>
class BenchmarkPhysicsSystem : public PhysicsSystem
{
public:
	BenchmarkPhysicsSystem(std::unique_ptr<PhysicsSystem>&& physicsSystem);
	// Inherited via PhysicsSystem
	void PhysicsUpdate(float deltaTime) override;
	void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
	void UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
	void UnregisterAllPhysicsComponents() override;
	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance) override;
	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore) override;
private:
	static constexpr size_t BENCHMARKAMOUNT{ 100 };
	std::deque<double> m_CalculatedTimes{};
	std::unique_ptr<PhysicsSystem> m_System;


	// Inherited via PhysicsSystem
	size_t GetPhysicsComponentCount() override;

};

