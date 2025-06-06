#pragma once
#include "PhysicsSystem.h"
#include "Observer.h"


//class QuadTreeSpatialPhysicsSystem : public PhysicsSystem
//{
//
//};

struct Node
{
	Engine::Rect bounds;
	std::vector<PhysicsComponent*> agents;

	std::unique_ptr<Node> children[4];
	



};

//class QuadTreeSpace
//{
//public:
//	void Insert(PhysicsComponent* agent)
//};
//
//class QuadTreeSpatialPhysicsSystem: public PhysicsSystem, public Observer
//{
//public:
//	QuadTreeSpatialPhysicsSystem(float screenWidth, float screenHeight);
//
//	// Inherited via PhysicsSystem
//	void PhysicsUpdate(float deltaTime) override;
//	void RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
//	void UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent) override;
//	virtual void DebugDraw() override;
//	void Notify(const Event& event, EventDispatcher* subject) override;
//
//	// Inherited via PhysicsSystem
//	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance) override;
//	std::optional<HitInfo> Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore);
//private:
//	std::unique_ptr<SpatialPartitioning::CellSpace> m_pCellSpace;
//	std::vector<PhysicsComponent*> m_pPhysicsAgents;
//	float m_NeighborHoodRange{ 150.f };
//
//	// Inherited via Observer
//
//
//};