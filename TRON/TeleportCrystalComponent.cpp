#include "TeleportCrystalComponent.h"
#include "PlayerHandlerComponent.h"

TeleportCrystalComponent::TeleportCrystalComponent(glm::vec2 bounds, Engine::Graph* graph):
	PhysicsComponent{ bounds },
	m_Graph{graph}
{
}

void TeleportCrystalComponent::OnCollide(float, PhysicsComponent& , dae::GameObject& agent, const HitInfo&)
{
	if (agent.GetComponent<PlayerTankHandlerComponent>())
	{
		Engine::GraphNode* randomNode = m_Graph->GetRandomNode();
		agent.SetPosition(randomNode->worldPosition.x, randomNode->worldPosition.y);
	}
}
