#include "SimpleSpatialPhysicsSystem.h"
#include <algorithm>
#include "EngineUtils.h"
#include "Event.h"
#include "Renderer.h"

// ------ Cell -----
SpatialPartitioning::Cell::Cell(float left, float bottom, float width, float height)
{
	m_BoundingBox.x = left;
	m_BoundingBox.y = bottom;
	m_BoundingBox.width = width;
	m_BoundingBox.height = height;
}

std::vector<glm::vec2> SpatialPartitioning::Cell::GetRectPoints() const
{
	auto left = m_BoundingBox.x;
	auto bottom = m_BoundingBox.y;
	auto width = m_BoundingBox.width;
	auto height = m_BoundingBox.height;

	std::vector<glm::vec2> points =
	{
		{left, bottom},
		{left, bottom + height},
		{left + width, bottom + height},
		{left + width, bottom}
	};
	return points;
}

// ------ CellSpace -----

SpatialPartitioning::CellSpace::CellSpace(float width, float height, int rows, int cols):
	m_SpaceWidth(width),
	m_SpaceHeight(height),
	m_NrOfRows(rows),
	m_NrOfCols(cols),
	m_CellWidth(width / cols),
	m_CellHeight(height / rows)
{
	m_CellWidth = width / cols;
	m_CellHeight = height / rows;

	for (int rowIdx{}; rowIdx < rows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < cols; ++colIdx)
		{
			m_Cells.emplace_back(colIdx * m_CellWidth,
				rowIdx * m_CellHeight,
				m_CellWidth,
				m_CellHeight);
		}
	}

	m_Neighbors.reserve(200);
}

void SpatialPartitioning::CellSpace::AddAgent(PhysicsComponent* const agent)
{
	m_Cells[PositionToIndex(agent->GetOwner()->GetTransform().GetWorldPosition())].m_pPhysicsAgents.push_back(agent);
}

void SpatialPartitioning::CellSpace::RemoveAgent(PhysicsComponent* const agent)
{
	const glm::vec2& pos = agent->GetOwner()->GetTransform().GetWorldPosition();
	const int index = PositionToIndex(pos);

	m_Cells[index].m_pPhysicsAgents.remove(agent);
}

void SpatialPartitioning::CellSpace::AgentPositionChanged(PhysicsComponent* const agent, const glm::vec2& oldPos)
{
	const int oldIndex = PositionToIndex(oldPos);
	const int newIndex = PositionToIndex(agent->GetOwner()->GetTransform().GetWorldPosition());
	
	if (newIndex == oldIndex) return;


	m_Cells[oldIndex].m_pPhysicsAgents.remove(agent);
	m_Cells[newIndex].m_pPhysicsAgents.emplace_back(agent);
}

void SpatialPartitioning::CellSpace::RegisterNeighbors(PhysicsComponent* const pAgent, float neighborhoodRadius)
{
	glm::vec2 agentPos = pAgent->GetPosition();
	m_NrOfNeighbors = 0;
	m_Neighbors.clear();
	m_NeighborhoodRadius = neighborhoodRadius;

	int minCol = std::max(0, int((agentPos.x - neighborhoodRadius) / m_CellWidth));
	int maxCol = std::min(m_NrOfCols - 1, int((agentPos.x + neighborhoodRadius) / m_CellWidth));
	int minRow = std::max(0, int((agentPos.y - neighborhoodRadius) / m_CellHeight));
	int maxRow = std::min(m_NrOfRows - 1, int((agentPos.y + neighborhoodRadius) / m_CellHeight));

	for (int row{ minRow }; row <= maxRow; ++row)
	{
		for (int col{ minCol }; col <= maxCol; ++col)
		{
			int cellIdx = row * m_NrOfCols + col;
			Cell& cell = m_Cells[cellIdx];


			if (Engine::CheckAABBCollision(cell.m_BoundingBox, Engine::Rect{ agentPos - glm::vec2(neighborhoodRadius, neighborhoodRadius), neighborhoodRadius * 2, neighborhoodRadius * 2 }))
			{
				continue;
			}

			for (const auto& agent : cell.m_pPhysicsAgents)
			{

				float distance = glm::distance(agent->GetPosition(), pAgent->GetPosition());
				if (distance < neighborhoodRadius)
				{
					m_Neighbors.emplace_back(agent);
				}
			}
		}
	}


}

void SpatialPartitioning::CellSpace::EmptyCells()
{
	for (Cell& c : m_Cells)
		c.m_pPhysicsAgents.clear();
}

void SpatialPartitioning::CellSpace::RenderCells() const
{
	for (int cellIndex{}; cellIndex < m_NrOfCols * m_NrOfRows; ++cellIndex)
	{
		dae::Renderer& renderer = dae::Renderer::GetInstance();
		renderer.DrawPolygon(m_Cells[cellIndex].GetRectPoints(), { 0,255,255,255 });

		renderer.DrawString(std::to_string(m_Cells[cellIndex].m_pPhysicsAgents.size()), dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", (uint8_t)(12)).get(), glm::vec2{ m_Cells[cellIndex].m_BoundingBox.x,m_Cells[cellIndex].m_BoundingBox.y } + glm::vec2(0, m_Cells[cellIndex].m_BoundingBox.height), { 255, 255, 255, 255 }, 12.f);
	}

}


int SpatialPartitioning::CellSpace::PositionToIndex(const glm::vec2& pos) const
{
	const int row = (int)(std::clamp(pos.y, 0.0f, m_SpaceHeight - 0.01f) / m_CellHeight);
	const int col = (int)(std::clamp(pos.x, 0.0f, m_SpaceWidth - 0.01f) / m_CellWidth);
	return (row * m_NrOfCols + col);
}

void SimpleSpatialPhysicsSystem::PhysicsUpdate(float fixedTime)
{
	for (auto& agent : m_pPhysicsAgents)
	{
		m_pCellSpace->RegisterNeighbors(agent, m_NeighborHoodRange);
	
		for (int neighborIndex{}; neighborIndex < m_pCellSpace->GetNeighbors().size(); ++neighborIndex)
		{
			PhysicsComponent* pNeighbor = m_pCellSpace->GetNeighbors()[neighborIndex];
			if (pNeighbor == agent) continue;
			//if (pNeighbor->HasCollided) continue;

			if (Engine::CheckAABBCollision(agent->GetBoundingBox(), pNeighbor->GetBoundingBox()))
			{
				const Engine::Rect& rectA = agent->GetBoundingBox();
				const Engine::Rect& rectB = pNeighbor->GetBoundingBox();

				glm::vec2 centerA = glm::vec2(rectA.x + rectA.width * 0.5f, rectA.y + rectA.height * 0.5f);
				glm::vec2 halfSizeA = glm::vec2(rectA.width * 0.5f, rectA.height * 0.5f);

				glm::vec2 centerB = glm::vec2(rectB.x + rectB.width * 0.5f, rectB.y + rectB.height * 0.5f);
				glm::vec2 halfSizeB = glm::vec2(rectB.width * 0.5f, rectB.height * 0.5f);

				glm::vec2 delta = centerA - centerB;
				glm::vec2 combinedHalfSize = halfSizeA + halfSizeB;
				glm::vec2 overlap = combinedHalfSize - glm::abs(delta);

				if (overlap.x > 0 && overlap.y > 0) 
				{
					HitInfo hitInfo;

					if (overlap.x < overlap.y)
					{
						hitInfo.penetrationDepth = overlap.x;
						hitInfo.normal = (delta.x < 0) ? glm::vec2(-1, 0) : glm::vec2(1, 0);
					}
					else
					{
						hitInfo.penetrationDepth = overlap.y;
						hitInfo.normal = (delta.y < 0) ? glm::vec2(0, -1) : glm::vec2(0, 1);
					}

					hitInfo.hitPosition = centerA; 

					agent->OnCollide(fixedTime, *pNeighbor, *pNeighbor->GetOwner(), hitInfo);
					//agent->HasCollided = true;
				}
			}

		}
	}

}

void SimpleSpatialPhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
	if (pPhysicsComponent == nullptr) return;
	
	pPhysicsComponent->GetOwner()->GetGameObjectEventDispatcher()->AddObserver(this);
	m_pPhysicsAgents.emplace_back(pPhysicsComponent);	
	m_pCellSpace->AddAgent(pPhysicsComponent);


}

SimpleSpatialPhysicsSystem::SimpleSpatialPhysicsSystem(float screenWidth, float screenHeight)
{

	m_pCellSpace = std::make_unique<SpatialPartitioning::CellSpace>(screenWidth, screenHeight, 20, 20);
}

void SimpleSpatialPhysicsSystem::UnregisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
	if (pPhysicsComponent == nullptr) return;

	auto it = std::find(m_pPhysicsAgents.begin(), m_pPhysicsAgents.end(), pPhysicsComponent);
	if (it != m_pPhysicsAgents.end())
	{
		m_pPhysicsAgents.erase(it);
	}
	m_pCellSpace->RemoveAgent(pPhysicsComponent);

}

void SimpleSpatialPhysicsSystem::DebugDraw()
{
	//m_pCellSpace->RenderCells();
	for (const auto& agent : m_pPhysicsAgents)
	{
		Engine::Rect box = agent->GetBoundingBox();
		dae::Renderer::GetInstance().DrawSquare((int) box.x, (int) box.y, (int) agent->GetWidth(), {0, 255, 0, 255});
	}
}

void SimpleSpatialPhysicsSystem::Notify(const Event& event, EventDispatcher*)
{
	if (event.eventType == EngineEvents::EVENT_GAMEOBJECT_TRANSFORMCHANGED)
	{
		EventContext::GameObjectTransformChangedEventContext ctx = std::any_cast<EventContext::GameObjectTransformChangedEventContext>(event.eventContext);

		m_pCellSpace->AgentPositionChanged(ctx.gameObject->GetComponent<PhysicsComponent>(), ctx.oldTransform.GetWorldPosition());
	}
}
