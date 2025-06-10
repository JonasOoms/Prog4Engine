#include "BenchmarkPhysicsSystem.h"
#include <chrono>
#include <algorithm>
#include <numeric>

BenchmarkPhysicsSystem::BenchmarkPhysicsSystem(std::unique_ptr<PhysicsSystem>&& physicsSystem):
	m_System{std::move(physicsSystem)}
{
}

void BenchmarkPhysicsSystem::PhysicsUpdate(float deltaTime)
{
	auto now = std::chrono::high_resolution_clock::now();
	m_System->PhysicsUpdate(deltaTime);
	auto end = std::chrono::high_resolution_clock::now();

	auto delta = std::chrono::duration<double, std::milli>(end - now).count();

	m_CalculatedTimes.emplace_back(delta);
	if (m_CalculatedTimes.size() > BENCHMARKAMOUNT)
	{
		m_CalculatedTimes.pop_front();
	}

	auto accumulatedTime = std::accumulate(m_CalculatedTimes.begin(), m_CalculatedTimes.end(), 0.0);
	auto averageTime = accumulatedTime / m_CalculatedTimes.size();

	std::cout << "AVERAGE PHYSICS UPDATE TIME: " << averageTime << " WITH " << m_System->GetPhysicsComponentCount() << " AGENTS" << std::endl;
}

void BenchmarkPhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
	m_System->RegisterPhysicsComponent(pPhysicsComponent);
}

void BenchmarkPhysicsSystem::UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
	m_System->UnregisterPhysicsComponent(pPhysicsComponent);
}

void BenchmarkPhysicsSystem::UnregisterAllPhysicsComponents()
{
	m_System->UnregisterAllPhysicsComponents();
}

std::optional<HitInfo> BenchmarkPhysicsSystem::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance)
{
	return std::move(m_System->Raycast(origin, direction, maxDistance));
}

std::optional<HitInfo> BenchmarkPhysicsSystem::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore)
{
	return std::move(m_System->Raycast(origin, direction, maxDistance, pIgnore));
}

size_t BenchmarkPhysicsSystem::GetPhysicsComponentCount()
{
	return m_System->GetPhysicsComponentCount();
}
