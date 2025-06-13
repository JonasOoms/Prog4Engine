#pragma once
#include "Component.h"
#include "GridComponent.h"
#include "Event.h"
#include "GameEvents.h"
#include "SimpleMoveCommand.h"
#include "IEnemy.h"
#include "ShootCommand.h"
#include "PhysicsComponent.h"

class AITankState;
class TankPatrolState;

class AITankComponent : public Component, public IEnemyHandler
{
public:
	AITankComponent(GridComponent* gridComponent, std::vector<dae::GameObject*>* target, float tankSpeed);

	void ComponentOwnerInitialized() override;
	virtual void Update(float) override;
	virtual void DebugDraw() override;

	SimpleMoveCommand* GetMoveCommand() { return m_MoveCommand.get();}
	EnemyShootCommand* GetShootCommand() { return m_ShootCommand.get();}

	PhysicsComponent* GetPhysicsComponent() { return m_CachedPhysicsComponent; }
	GridComponent* GetGridComponent() { return m_Grid; }
	dae::GameObject* GetTarget(int index) { return m_Target->at(index); }
	size_t GetTargetCount() { return m_Target->size(); }
	float GetSpeed() { return m_Speed; }
	EventDispatcher* GetAITankEvent();

	// Inherited via IEnemyHandler
	EventDispatcher* GetEnemyEventDispatcher() override;
private:
	PhysicsComponent* m_CachedPhysicsComponent{};
	std::unique_ptr<SimpleMoveCommand> m_MoveCommand;
	std::unique_ptr<EnemyShootCommand> m_ShootCommand;
	std::unique_ptr<AITankState> m_State;
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
	float m_Speed;
	GridComponent* m_Grid;
	std::vector<dae::GameObject*>* m_Target;
	int m_HitPoints{ 5 };
	bool m_IsReady{ false };

	// Inherited via IEnemyHandlerComponent
	void Hurt(int damage) override;
	int GetHealth() override;
};

class AITankState
{
public:
	virtual ~AITankState() = default;
	virtual std::unique_ptr<AITankState> Update(AITankComponent& AiTankComponent, float ds) = 0;
	virtual void DebugDraw(AITankComponent&) {};
};

class TankPatrolState : public AITankState
{
public:
	TankPatrolState() = default;
	virtual std::unique_ptr<AITankState> Update(AITankComponent& AiTankComponent, float ds) override;
	virtual void DebugDraw(AITankComponent& AiTankComponent) override;
private:
	PhysicsComponent* m_PhysicsComponent{};
	bool WasCurrentTravelPointReached(AITankComponent& AiTankComponent);
	Engine::GraphNode* m_SelectedTravelPoint{};
	std::queue<Engine::GraphNode*> m_Path;
	
};

class TankEngageState : public AITankState
{
public:
	TankEngageState() = default;
	virtual std::unique_ptr<AITankState> Update(AITankComponent& AiTankComponent, float ds) override;
	virtual void DebugDraw(AITankComponent& AiTankComponent) override;
private:
	PhysicsComponent* m_PhysicsComponent{};
	bool WasCurrentTravelPointReached(AITankComponent& AiTankComponent);
	Engine::GraphNode* m_SelectedTravelPoint{};
	std::queue<Engine::GraphNode*> m_Path;
};

class TankShootState : public AITankState
{
public:
	TankShootState() = default;
	virtual std::unique_ptr<AITankState> Update(AITankComponent& AiTankComponent, float ds) override;
	virtual void DebugDraw(AITankComponent& AiTankComponent) override;
private:
	float m_ShootCountdown{};
	const float m_SecondsPerShot{.5f};
};