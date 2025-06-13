#include "AiRecogniserComponent.h"
#include "PhysicsSystem.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"
#include "GameEvents.h"

RecogniserCollisionComponent::RecogniserCollisionComponent(const glm::vec2& size):
	PhysicsComponent(size)
{
}

void RecogniserCollisionComponent::OnCollide(float , PhysicsComponent&, dae::GameObject& other, const HitInfo&)
{
	if (PlayerTankHandlerComponent* handler = other.GetComponent<PlayerTankHandlerComponent>())
	{
		handler->Hurt();
		GetOwner()->Destroy();
	}
}

AIRecogniserComponent::AIRecogniserComponent(GridComponent* gridComponent, std::vector<dae::GameObject*>* target, float tankSpeed):
m_Grid{ gridComponent },
m_Target{ target },
m_Speed{ tankSpeed }
{
	m_State = std::make_unique<RecogniserPatrolState>();
	m_EventDispatcher = std::make_unique<EventDispatcher>();
}

void AIRecogniserComponent::ComponentOwnerInitialized()
{
	m_MoveCommand = std::make_unique<SimpleMoveCommand>(GetOwner(), glm::vec2{ -1,0 }, m_Speed);
	m_CachedPhysicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
	m_IsReady = true;
}

void AIRecogniserComponent::Update(float ds)
{
	if (m_IsReady)
	{
		if (auto newState = m_State->Update(*this, ds)) {
			m_State = std::move(newState);
		}
	}
}

void AIRecogniserComponent::DebugDraw()
{
}

EventDispatcher* AIRecogniserComponent::GetAITankEvent()
{
	return m_EventDispatcher.get();
}

EventDispatcher* AIRecogniserComponent::GetEnemyEventDispatcher()
{
	return m_EventDispatcher.get();
}

void AIRecogniserComponent::Hurt(int damage)
{
	m_HitPoints -= damage;
	if (m_HitPoints <= 0)
	{
		ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Explosion"), 20.f);
		Event event{ TRONEvents::EVENT_ENEMY_DEATH, TRONEvents::EventContexts::EnemyDeathEventContext{GetOwner()} };
		m_EventDispatcher->NotifyObservers(event);
		GetOwner()->Destroy();
	}
}

int AIRecogniserComponent::GetHealth()
{
	return m_HitPoints;
}

std::unique_ptr<AIRecogniserState> RecogniserPatrolState::Update(AIRecogniserComponent& AiTankComponent, float)
{
	PhysicsSystem* ps = ServiceLocator::GetPhysicsSystem();

	PhysicsComponent* physicsComponent = AiTankComponent.GetPhysicsComponent();

	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		dae::GameObject* player = AiTankComponent.GetTarget(playerIndex);
		if (player)
		{
			PhysicsComponent* targetPhysicsComponent = player->GetComponent<PhysicsComponent>();

			assert(physicsComponent && "The enemy tank must have a PhysicsComponent!");
			assert(targetPhysicsComponent && "The player tank must have a PhysicsComponent!");

			std::optional<HitInfo> optHitInfo = ps->Raycast(physicsComponent->GetPosition(), targetPhysicsComponent->GetPosition() - physicsComponent->GetPosition(), 400, physicsComponent);

			if (optHitInfo.has_value())
			{
				if (optHitInfo->pHitComponent == targetPhysicsComponent)
				{
					return std::make_unique<RecogniserAttackState>();
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

bool RecogniserPatrolState::WasCurrentTravelPointReached(AIRecogniserComponent& AiTankComponent)
{
	constexpr float minDistance = 1.f;
	glm::vec2 currentPosition = AiTankComponent.GetOwner()->GetPosition();
	glm::vec2 targetPosition = m_SelectedTravelPoint->worldPosition;
	return (glm::distance(currentPosition, targetPosition) < minDistance);
}

std::unique_ptr<AIRecogniserState> RecogniserAttackState::Update(AIRecogniserComponent& AiTankComponent, float )
{
	PhysicsSystem* ps = ServiceLocator::GetPhysicsSystem();
	PhysicsComponent* physicsComponent = AiTankComponent.GetPhysicsComponent();

	for (int playerIndex{}; playerIndex < AiTankComponent.GetTargetCount(); ++playerIndex)
	{
		PhysicsComponent* targetPhysicsComponent = AiTankComponent.GetTarget(playerIndex)->GetComponent<PhysicsComponent>();
		assert(physicsComponent && targetPhysicsComponent);

		glm::vec2 tankPos = physicsComponent->GetPosition();
		glm::vec2 targetPos = targetPhysicsComponent->GetPosition();
		glm::vec2 toTarget = targetPos - tankPos;

		std::optional<HitInfo> optHitInfo = ps->Raycast(tankPos, toTarget, 400.f, physicsComponent);

		if (optHitInfo && optHitInfo->pHitComponent == targetPhysicsComponent)
		{
			
				if (m_Path.empty() && m_SelectedTravelPoint == nullptr)
				{
					Engine::Graph* pGraph = AiTankComponent.GetGridComponent()->GetGraphFromGrid();
					Engine::GraphNode* pStart = pGraph->GetClosestNode(AiTankComponent.GetOwner()->GetPosition());
					Engine::GraphNode* pEnd = pGraph->GetClosestNode(targetPos);

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
		}
	return std::make_unique<RecogniserPatrolState>();
}

bool RecogniserAttackState::WasCurrentTravelPointReached(AIRecogniserComponent& AiTankComponent)
{
	constexpr float minDistance = 1.f;
	glm::vec2 currentPosition = AiTankComponent.GetOwner()->GetPosition();
	glm::vec2 targetPosition = m_SelectedTravelPoint->worldPosition;
	return (glm::distance(currentPosition, targetPosition) < minDistance);
}

