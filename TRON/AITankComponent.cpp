#include "AITankComponent.h"
#include <queue>
#include <map>
#include "ServiceLocator.h"
#include "PhysicsSystem.h"
#include <iostream>
#include "Graph.h"
#include "Renderer.h"
#include "Minigin.h"
#include "SoundSystem.h"
#include "GameRegistries.h"

AITankComponent::AITankComponent(GridComponent* gridComponent, std::vector<dae::GameObject*>* target, float tankSpeed) :
	m_Grid{ gridComponent },
	m_Target{ target },
	m_Speed{ tankSpeed }
{
	m_State = std::make_unique<TankPatrolState>();
	m_EventDispatcher = std::make_unique<EventDispatcher>();
}

void AITankComponent::ComponentOwnerInitialized()
{
	m_MoveCommand = std::make_unique<SimpleMoveCommand>(GetOwner(), glm::vec2{ -1,0 }, m_Speed);
	m_IsReady = true;
}

void AITankComponent::Update(float ds)
{
	if (m_IsReady)
	{
		if (auto newState = m_State->Update(*this, ds)) {
			m_State = std::move(newState);
		}
	}
}

void AITankComponent::DebugDraw()
{
	if (m_IsReady)
	{
		m_State->DebugDraw(*this);
	}
}

EventDispatcher* AITankComponent::GetAITankEvent()
{
	return m_EventDispatcher.get();
}

void AITankComponent::Hurt(int damage)
{
	m_HitPoints -= damage;
	if (m_HitPoints <= 0)
	{
		ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Explosion"), 20.f);
		GetOwner()->Destroy();
	}
}

int AITankComponent::GetHealth()
{
	return m_HitPoints;
}


std::unique_ptr<AITankState> TankPatrolState::Update(AITankComponent& AiTankComponent, float)
{

	PhysicsSystem* ps = ServiceLocator::GetPhysicsSystem();
	// Step 1: Racyast to player. If hit, continue to shooting state. 

	PhysicsComponent* physicsComponent = AiTankComponent.GetOwner()->GetComponent<PhysicsComponent>();
	
	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		dae::GameObject* player = AiTankComponent.GetTarget(playerIndex);
		if (player)
		{
			PhysicsComponent* targetPhysicsComponent = player->GetComponent<PhysicsComponent>();

			assert(physicsComponent && "The enemy tank must have a PhysicsComponent!");
			assert(targetPhysicsComponent && "The player tank must have a PhysicsComponent!");

			std::optional<HitInfo> optHitInfo = ps->Raycast(physicsComponent->GetPosition(), targetPhysicsComponent->GetPosition() - physicsComponent->GetPosition(), 200, physicsComponent);

			if (optHitInfo.has_value())
			{
				if (optHitInfo->pHitComponent == targetPhysicsComponent)
				{
					return std::make_unique<TankEngageState>();
				}
			}
		}
	}

	if (m_Path.empty() && m_SelectedTravelPoint == nullptr)
	{
		Engine::Graph* pGraph = AiTankComponent.GetGridComponent()->GetGraphFromGrid();
		Engine::GraphNode* pStart = pGraph->GetClosestNode(AiTankComponent.GetOwner()->GetPosition());
		Engine::GraphNode* pEnd = pGraph->GetRandomNode();

		std::vector<Engine::GraphNode*> pathVector = Engine::BFSPathSearch(pStart, pEnd);
		//std::reverse(pathVector.begin(), pathVector.end());
		for (Engine::GraphNode* node : pathVector)
		{
			m_Path.push(node);
		}

		m_SelectedTravelPoint = m_Path.front();
		m_Path.pop();
	}
	else
	{
		if (WasCurrentTravelPointReached(AiTankComponent))
		{
			if (!m_Path.empty())
			{
				m_SelectedTravelPoint = m_Path.front();
				m_Path.pop();
			}
			else
			{
				m_SelectedTravelPoint = nullptr;
			}
		}
		else
		{
			glm::vec2 currentPosition = AiTankComponent.GetOwner()->GetPosition();
			glm::vec2 targetPosition = m_SelectedTravelPoint->worldPosition;
			glm::vec2 delta = glm::normalize(targetPosition - currentPosition);
			
			SimpleMoveCommand* moveCommand = AiTankComponent.GetMoveCommand();
			moveCommand->SetMovementVector(delta);
			moveCommand->Execute();
			

			TRONEvents::EventContexts::PlayerMoveEventContext context{};
			context.movementDelta = delta;
			context.newPosition = AiTankComponent.GetOwner()->GetPosition();

			Event event;
			event.eventType = TRONEvents::EVENT_PLAYER_MOVE;
			event.eventContext = context;

			AiTankComponent.GetAITankEvent()->NotifyObservers(event);


		}
	}

	return nullptr;

}

void TankPatrolState::DebugDraw(AITankComponent& AiTankComponent)
{

	dae::Renderer& renderer = dae::Renderer::GetInstance();

	std::queue<Engine::GraphNode*> copy = m_Path;
	std::vector<Engine::GraphNode*> vectorPath;
	vectorPath.reserve(copy.size());
	while (!copy.empty())
	{
		vectorPath.emplace_back(copy.front());
		copy.pop();
	}

	for (Engine::GraphNode* node : vectorPath)
	{
		renderer.DrawPoint(static_cast<int>(node->worldPosition.x), static_cast<int>(node->worldPosition.y), SDL_Color{ 255,255,0,255 });
	}

	PhysicsComponent* physicsComponent = AiTankComponent.GetOwner()->GetComponent<PhysicsComponent>();
	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		PhysicsComponent* targetPhysicsComponent = AiTankComponent.GetTarget(playerIndex)->GetComponent<PhysicsComponent>();

		assert(physicsComponent && "The enemy tank must have a PhysicsComponent!");
		assert(targetPhysicsComponent && "The player tank must have a PhysicsComponent!");

		glm::vec2 position = physicsComponent->GetPosition();
		glm::vec2 target = targetPhysicsComponent->GetPosition();
		glm::vec2 delta = position + glm::normalize(target - position) * 200.f;
		renderer.drawLine(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(delta.x), static_cast<int>(delta.y), SDL_Color{ 255, 0, 0, 255 });
	}

}

bool TankPatrolState::WasCurrentTravelPointReached(AITankComponent& AiTankComponent)
{
	constexpr float minDistance = 1.f;
	glm::vec2 currentPosition = AiTankComponent.GetOwner()->GetPosition();
	glm::vec2 targetPosition = m_SelectedTravelPoint->worldPosition;
	return (glm::distance(currentPosition, targetPosition) < minDistance);
}

std::unique_ptr<AITankState> TankEngageState::Update(AITankComponent& AiTankComponent, float )
{
	// Step 1. Check if I can see the player
	PhysicsSystem* ps = ServiceLocator::GetPhysicsSystem();
	PhysicsComponent* physicsComponent = AiTankComponent.GetOwner()->GetComponent<PhysicsComponent>();
	
	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		PhysicsComponent* targetPhysicsComponent = AiTankComponent.GetTarget(playerIndex)->GetComponent<PhysicsComponent>();

		assert(physicsComponent && "The enemy tank must have a PhysicsComponent!");
		assert(targetPhysicsComponent && "The player tank must have a PhysicsComponent!");

		std::optional<HitInfo> optHitInfo = ps->Raycast(physicsComponent->GetPosition(), targetPhysicsComponent->GetPosition() - physicsComponent->GetPosition(), 200, physicsComponent);

		if (optHitInfo.has_value())
		{
			if (optHitInfo->pHitComponent == targetPhysicsComponent)
			{
				return nullptr;
			}
		}
		// Step 2. Shoot at the player.
		
	}
	return std::make_unique<TankPatrolState>();
}

void TankEngageState::DebugDraw(AITankComponent& AiTankComponent)
{
	dae::Renderer& renderer = dae::Renderer::GetInstance();
	PhysicsComponent* physicsComponent = AiTankComponent.GetOwner()->GetComponent<PhysicsComponent>();
	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		dae::GameObject* player = AiTankComponent.GetTarget(playerIndex);
		PhysicsComponent* targetPhysicsComponent = player->GetComponent<PhysicsComponent>();

		assert(physicsComponent && "The enemy tank must have a PhysicsComponent!");
		assert(targetPhysicsComponent && "The player tank must have a PhysicsComponent!");

		glm::vec2 position = physicsComponent->GetPosition();
		glm::vec2 target = targetPhysicsComponent->GetPosition();
		glm::vec2 delta = position + glm::normalize(target - position) * 200.f;
		renderer.drawLine(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(delta.x), static_cast<int>(delta.y), SDL_Color{ 255, 255, 0, 255 });
	}

	//std::optional<HitInfo> optHitInfo = ps->Raycast(physicsComponent->GetPosition(), targetPhysicsComponent->GetPosition() - physicsComponent->GetPosition(), 200, physicsComponent);
}
