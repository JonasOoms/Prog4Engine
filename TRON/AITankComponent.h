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
	AITankComponent(GridComponent* gridComponent, std::vector<dae::GameObject*>* target, float tankSpeed);

	void ComponentOwnerInitialized() override;
	virtual void Update(float) override;
	virtual void DebugDraw() override;

	SimpleMoveCommand* GetMoveCommand() { return m_MoveCommand.get();}

	GridComponent* GetGridComponent() { return m_Grid; }
	dae::GameObject* GetTarget(int index) { return m_Target->at(index); }
	size_t GetTargetCount() { return m_Target->size(); }
	float GetSpeed() { return m_Speed; }
	EventDispatcher* GetAITankEvent();
private:
	std::unique_ptr<SimpleMoveCommand> m_MoveCommand;
	std::unique_ptr<AITankState> m_State;
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
	float m_Speed;
	GridComponent* m_Grid;
	std::vector<dae::GameObject*>* m_Target;
	bool m_IsReady{ false };
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
};