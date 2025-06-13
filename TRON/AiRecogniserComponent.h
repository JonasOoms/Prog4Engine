#pragma once
#include <Component.h>
#include "IEnemy.h"
#include "GridComponent.h"
#include "SimpleMoveCommand.h"
#include "PhysicsComponent.h"
#include "PlayerHandlerComponent.h"

class RecogniserCollisionComponent : public PhysicsComponent
{
public:
	RecogniserCollisionComponent(const glm::vec2& size);
	virtual ~RecogniserCollisionComponent() = default;
	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
};

class AIRecogniserComponent;

class AIRecogniserState
{
public:
	virtual ~AIRecogniserState() = default;
	virtual std::unique_ptr<AIRecogniserState> Update(AIRecogniserComponent& AiTankComponent, float ds) = 0;
};


class RecogniserPatrolState : public AIRecogniserState
{
public:
	RecogniserPatrolState() = default;
	// Inherited via AITankState
	std::unique_ptr<AIRecogniserState> Update(AIRecogniserComponent& AiTankComponent, float ds) override;
private:
	bool WasCurrentTravelPointReached(AIRecogniserComponent& AiTankComponent);
	Engine::GraphNode* m_SelectedTravelPoint{};
	std::queue<Engine::GraphNode*> m_Path;
};

class RecogniserAttackState : public AIRecogniserState
{
public:
	RecogniserAttackState() = default;
	// Inherited via AIRecogniserState
	std::unique_ptr<AIRecogniserState> Update(AIRecogniserComponent& AiTankComponent, float ds) override;
private:
	bool WasCurrentTravelPointReached(AIRecogniserComponent& AiTankComponent);
	Engine::GraphNode* m_SelectedTravelPoint{};
	std::queue<Engine::GraphNode*> m_Path;
};

class AIRecogniserComponent : public Component, public IEnemyHandler
{
public:
	AIRecogniserComponent(GridComponent* gridComponent, std::vector<dae::GameObject*>* target, float tankSpeed);

	void ComponentOwnerInitialized() override;
	virtual void Update(float) override;
	virtual void DebugDraw() override;

	SimpleMoveCommand* GetMoveCommand() { return m_MoveCommand.get(); }
	

	PhysicsComponent* GetPhysicsComponent() { return m_CachedPhysicsComponent; }
	GridComponent* GetGridComponent() { return m_Grid; }
	dae::GameObject* GetTarget(int index) { return m_Target->at(index); }
	size_t GetTargetCount() { return m_Target->size(); }
	float GetSpeed() { return m_Speed; }
	EventDispatcher* GetAITankEvent();

	// Inherited via IEnemyHandler
	EventDispatcher* GetEnemyEventDispatcher() override;
	void Hurt(int damage) override;
	int GetHealth() override;
private:

	PhysicsComponent* m_CachedPhysicsComponent{};

	std::unique_ptr<SimpleMoveCommand> m_MoveCommand;
	std::unique_ptr<AIRecogniserState> m_State;
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
	float m_Speed;
	GridComponent* m_Grid;
	std::vector<dae::GameObject*>* m_Target;
	int m_HitPoints{ 3 };
	bool m_IsReady{ false };

};
