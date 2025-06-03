#pragma once
#include "Component.h"
#include "GridComponent.h"
#include "Event.h"
#include "GameEvents.h"
#include "SimpleMoveCommand.h"

class AITankState;
class TankPatrolState;

class AITankComponent : public Component
{
public:
	AITankComponent(GridComponent* gridComponent, dae::GameObject* target, float tankSpeed);

	virtual void Update(float) override;
	virtual void DebugDraw() override;

	GridComponent* GetGridComponent() { return m_Grid; }
	dae::GameObject* GetTarget() { return m_Target; }
	float GetSpeed() { return m_Speed; }
	EventDispatcher* GetAITankEvent();
private:
	std::unique_ptr<SimpleMoveCommand> m_MoveCommand;
	std::unique_ptr<AITankState> m_State;
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
	float m_Speed;
	GridComponent* m_Grid;
	dae::GameObject* m_Target;
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

};