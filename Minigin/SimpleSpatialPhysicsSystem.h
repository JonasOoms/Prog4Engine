#pragma once

#include "PhysicsSystem.h"
#include "Observer.h"
#include <list>
#include <optional>

namespace SpatialPartitioning
{
	struct Cell
	{
		Cell(float left, float bottom, float width, float height);

		std::vector<glm::vec2> GetRectPoints() const;

		std::list<PhysicsComponent*> m_pPhysicsAgents;
		Engine::Rect m_BoundingBox;

	};

	class CellSpace
	{
	public:
		CellSpace(float width, float height, int rows, int cols);

		void AddAgent(PhysicsComponent* const agent);
		void RemoveAgent(PhysicsComponent* const agent);
		void AgentPositionChanged(PhysicsComponent* const agent, const glm::vec2& oldPos);

		void RegisterNeighbors(PhysicsComponent* const pAgent, float neighborhoodRadius);

		const std::vector<PhysicsComponent*>& GetNeighbors() const { return m_Neighbors; }
		int GetNrOfNeighbors() const { return m_NrOfNeighbors; }

		//bool IsAgentInCell(const Cell& cell, PhysicsComponent* agent) const;

		//empties the cells of entities
		void EmptyCells();

		void RenderCells() const;

	private:
		// Cells and properties
		std::vector<Cell> m_Cells;

		float m_SpaceWidth;
		float m_SpaceHeight;

		int m_NrOfRows;
		int m_NrOfCols;

		float m_CellWidth;
		float m_CellHeight;


		std::vector<PhysicsComponent*> m_Neighbors;
		int m_NrOfNeighbors;

		float m_NeighborhoodRadius{};

		// Helper functions
		int PositionToIndex(const glm::vec2& pos) const;

	};

}
class SimpleSpatialPhysicsSystem : public PhysicsSystem, public Observer
{
public:
	SimpleSpatialPhysicsSystem(float screenWidth, float screenHeight);

	// Inherited via PhysicsSystem
	void PhysicsUpdate(float deltaTime) override;
	void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
	void UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
	void UnregisterAllPhysicsComponents() override;
	virtual void DebugDraw() override;
	void Notify(const Event& event, EventDispatcher* subject) override;
private:
	std::unique_ptr<SpatialPartitioning::CellSpace> m_pCellSpace;
	std::vector<PhysicsComponent*> m_pPhysicsAgents;
	float m_NeighborHoodRange{ 300.f };

	// Inherited via Observer


	// Inherited via PhysicsSystem
	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance) override;
	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore);


	// Inherited via PhysicsSystem
	size_t GetPhysicsComponentCount() override;

};

