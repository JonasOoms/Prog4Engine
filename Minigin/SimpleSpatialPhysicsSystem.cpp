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

SpatialPartitioning::CellSpace::CellSpace(float width, float height, int rows, int cols) :
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
		if (!agent->GetIsStatic())
		{
			const Engine::Rect& rect = agent->GetBoundingBox();
			glm::vec2 velocity = agent->GetVelocity() * fixedTime;

			bool hitDetected = false;
			Engine::SweepResult bestResult;
			PhysicsComponent* hitComponent = nullptr;

			// sweep to all static colliders, finding the shortest path to collision if one exists
			for (auto& other : m_pPhysicsAgents)
			{
				if (other == agent || !other->GetIsStatic())
					continue;

				const Engine::Rect& otherRect = other->GetBoundingBox();
				Engine::SweepResult sweep = SweptAABB(rect, velocity, otherRect);
				if (sweep.hit && (!hitDetected || sweep.time < bestResult.time))
				{
					bestResult = sweep;
					hitDetected = true;
					hitComponent = other;
				}
			}

			// no hit detected, we can keep going
			if (!hitDetected)
			{
			
				agent->GetOwner()->SetPosition(rect.x + velocity.x, rect.y + velocity.y);
			}
			else
			{
				// hit was detected, lets make sure we block movement
				glm::vec2 safeMove = velocity * bestResult.time;
				constexpr float safeMargin = 0.001f;
				glm::vec2 safePos = glm::vec2(rect.x, rect.y) + safeMove - bestResult.normal * safeMargin;

				agent->GetOwner()->SetPosition(safePos.x, safePos.y);

				HitInfo info;
				info.hitPosition = safePos;
				info.normal = bestResult.normal;
				info.pHitComponent = hitComponent;

				// We are not in the collider, so no penetrationDepth
				info.penetrationDepth = 0.0f;

				agent->OnCollide(fixedTime, *hitComponent, *hitComponent->GetOwner(), info);
			}
		}
	}


	for (auto& agent : m_pPhysicsAgents)
	{
		m_pCellSpace->RegisterNeighbors(agent, m_NeighborHoodRange);

		for (int neighborIndex{}; neighborIndex < m_pCellSpace->GetNeighbors().size(); ++neighborIndex)
		{
			PhysicsComponent* pNeighbor = m_pCellSpace->GetNeighbors()[neighborIndex];
			if (pNeighbor == agent) continue;

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

				
					glm::vec2 normal(0.0f);

					if (overlap.x < overlap.y)
					{
					
						float penetrationDirection = delta.x < 0 ? -1.0f : 1.0f;
						normal.x = penetrationDirection;
						hitInfo.penetrationDepth = overlap.x;
					}
					else
					{
					
						float penetrationDirection = delta.y < 0 ? -1.0f : 1.0f;
						normal.y = penetrationDirection;
						hitInfo.penetrationDepth = overlap.y;
					}

				
					const float cornerThreshold = 0.25f;
					float ratio = overlap.x / overlap.y;

					if (ratio > 1.0f - cornerThreshold && ratio < 1.0f + cornerThreshold)
					{
					
						normal = glm::normalize(delta);
					}

					hitInfo.normal = normal;
					hitInfo.hitPosition = centerA + (halfSizeA * normal);

					if (pNeighbor->GetIsStatic() && hitInfo.penetrationDepth > 0.0f && glm::length(hitInfo.normal) > 0.0f)
					{
						glm::vec2 currentPosition = agent->GetOwner()->GetPosition();
						glm::vec2 correction = hitInfo.normal * hitInfo.penetrationDepth;
						glm::vec2 newPosition = currentPosition + correction;
						if (!agent->GetIsStatic())
						{
							agent->GetOwner()->SetPosition(newPosition.x, newPosition.y);
						}
					}

					agent->OnCollide(fixedTime, *pNeighbor, *pNeighbor->GetOwner(), hitInfo);
				}
			}

		}
	}

	for (auto& agent : m_pPhysicsAgents)
	{
		agent->HasCollided = false;
	}

}

void SimpleSpatialPhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* pPhysicsComponent)
{
	if (pPhysicsComponent == nullptr) return;

	auto it = std::find(m_pPhysicsAgents.begin(), m_pPhysicsAgents.end(), pPhysicsComponent);
	if (it != m_pPhysicsAgents.end()) return;

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

/// <summary>
/// Dangerous! Make sure you know what youre doing
/// </summary>
void SimpleSpatialPhysicsSystem::UnregisterAllPhysicsComponents()
{
	m_pPhysicsAgents.clear();
}

void SimpleSpatialPhysicsSystem::DebugDraw()
{
	//m_pCellSpace->RenderCells();
	/*for (const auto& agent : m_pPhysicsAgents)
	{

		Engine::Rect box = agent->GetBoundingBox();
		dae::Renderer::GetInstance().DrawRect(box, { 0, 255, 0, 255 });

		glm::vec2 pos = agent->GetOwner()->GetTransform().GetWorldPosition();
	}*/
}

void SimpleSpatialPhysicsSystem::Notify(const Event& event, EventDispatcher*)
{
	if (event.eventType == EngineEvents::EVENT_GAMEOBJECT_TRANSFORMCHANGED)
	{
		EventContext::GameObjectTransformChangedEventContext ctx = std::any_cast<EventContext::GameObjectTransformChangedEventContext>(event.eventContext);

		m_pCellSpace->AgentPositionChanged(ctx.gameObject->GetComponent<PhysicsComponent>(), ctx.oldTransform.GetWorldPosition());
	}
}

std::optional<HitInfo> SimpleSpatialPhysicsSystem::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance)
{
	glm::vec2 dir = glm::normalize(direction);
	glm::vec2 invDir = 1.0f / dir;

	std::optional<HitInfo> closestHit;
	float closestDistance = maxDistance;

	for (PhysicsComponent* agent : m_pPhysicsAgents)
	{
		const Engine::Rect& rect = agent->GetBoundingBox();

		glm::vec2 min = { rect.x, rect.y };
		glm::vec2 max = { rect.x + rect.width, rect.y + rect.height };

		glm::vec2 t1 = (min - origin) * invDir;
		glm::vec2 t2 = (max - origin) * invDir;

		glm::vec2 tmin = glm::min(t1, t2);
		glm::vec2 tmax = glm::max(t1, t2);

		float tNear = std::max(tmin.x, tmin.y);
		float tFar = std::min(tmax.x, tmax.y);

		if (tNear <= tFar && tFar >= 0 && tNear <= closestDistance)
		{
			closestDistance = tNear;

			HitInfo hit;
			hit.hitPosition = origin + dir * tNear;
			hit.penetrationDepth = 0.f;
			hit.pHitComponent = agent;
			glm::vec2 normal(0.f);

			if (tNear == tmin.x)
			{
				if (dir.x < 0)
					normal = glm::vec2(1.f, 0.f);   
				else
					normal = glm::vec2(-1.f, 0.f); 
			}
			else if (tNear == tmin.y)
			{
				
				if (dir.y < 0)
					normal = glm::vec2(0.f, 1.f);   
				else
					normal = glm::vec2(0.f, -1.f); 
			}


			closestHit = hit;
		}
	}

	return closestHit;
}

std::optional<HitInfo> SimpleSpatialPhysicsSystem::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, PhysicsComponent* pIgnore)
{
	glm::vec2 dir = glm::normalize(direction);
	glm::vec2 invDir = 1.0f / dir;

	std::optional<HitInfo> closestHit;
	float closestDistance = maxDistance;

	for (PhysicsComponent* agent : m_pPhysicsAgents)
	{
		if (agent == pIgnore) continue;

		const Engine::Rect& rect = agent->GetBoundingBox();

		glm::vec2 min = { rect.x, rect.y };
		glm::vec2 max = { rect.x + rect.width, rect.y + rect.height };

		glm::vec2 t1 = (min - origin) * invDir;
		glm::vec2 t2 = (max - origin) * invDir;

		glm::vec2 tmin = glm::min(t1, t2);
		glm::vec2 tmax = glm::max(t1, t2);

		float tNear = std::max(tmin.x, tmin.y);
		float tFar = std::min(tmax.x, tmax.y);

		if (tNear <= tFar && tFar >= 0 && tNear <= closestDistance)
		{
			closestDistance = tNear;

			HitInfo hit;
			hit.hitPosition = origin + dir * tNear;
			hit.penetrationDepth = 0.f;
			hit.pHitComponent = agent;

			glm::vec2 normal(0.f);
			if (tNear == tmin.x && tNear != tmin.y)
			{
				normal = (invDir.x < 0) ? glm::vec2(1.f, 0.f) : glm::vec2(-1.f, 0.f);
			}
			else if (tNear == tmin.y && tNear != tmin.x)
			{
				normal = (invDir.y < 0) ? glm::vec2(0.f, 1.f) : glm::vec2(0.f, -1.f);
			}
			else
			{
				
				glm::vec2 cornerNormal = glm::normalize(glm::vec2(
					(invDir.x < 0) ? 1.f : -1.f,
					(invDir.y < 0) ? 1.f : -1.f
				));
				normal = cornerNormal;
			}
			hit.normal = normal;
			closestHit = hit;
		}

	}

	return closestHit;
}

size_t SimpleSpatialPhysicsSystem::GetPhysicsComponentCount()
{
	return m_pPhysicsAgents.size();
}
