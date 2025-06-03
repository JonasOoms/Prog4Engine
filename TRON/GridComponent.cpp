#include "GridComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "PhysicsComponent.h"
#include <algorithm>

GridComponent::GridComponent(float width, float height, int rows, int columns):
	Component(),
	m_Width{width},
	m_Height{height},
	m_Rows{rows},
	m_Columns{columns},
	m_CellWidth{m_Width/float(m_Columns)},
	m_CellHeight{m_Height/float(m_Rows)}
{

	m_Grid.resize(rows);
	for (int rowIndex{}; rowIndex < rows; ++rowIndex)
	{
		m_Grid[rowIndex].resize(columns);
	}

	

}

void GridComponent::ComponentOwnerInitialized()
{
	GetOwner()->GetGameObjectEventDispatcher()->AddObserver(this);
}

void GridComponent::Notify(const Event& event, EventDispatcher*)
{
	switch (event.eventType)
	{
	case EngineEvents::EVENT_GAMEOBJECT_CHILDADDED:
		Add(
			std::any_cast<EventContext::GameObjectChildHierarchyChangedContext>(event.eventContext)
			.child
		);
		break;
	case EngineEvents::EVENT_GAMEOBJECT_CHILDREMOVED:
		Remove(
			std::any_cast<EventContext::GameObjectChildHierarchyChangedContext>(event.eventContext)
			.child
		);
		break;
	}
}



void GridComponent::Add(dae::GameObject* object)
{
	for (int rowIndex{}; rowIndex < m_Rows; ++rowIndex)
	{
		for (int columnIndex{}; columnIndex < m_Columns; ++columnIndex)
		{
			if (Get(rowIndex, columnIndex) == nullptr)
			{
				Insert(object, rowIndex, columnIndex);
				m_IsGraphValid = false;
				return;
			}
		}
	}
}

void GridComponent::Remove(dae::GameObject* object)
{
	for (int row = 0; row < m_Rows; ++row)
	{
		for (int column = 0; column < m_Columns; ++column)
		{
			if (Get(row, column) == object)
			{
				m_Grid[row][column] = nullptr;
				m_IsGraphValid = false;
				return;
			}
		}
	}
}

void GridComponent::Insert(dae::GameObject* object, int row, int column)
{
	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Columns)
	{
		return;
	}

	if (Get(row, column) == nullptr)
	{
		m_Grid[row][column] = object;
	}

	float x = column * m_CellWidth;
	float y = row * m_CellHeight;
	object->SetPosition(x, y);
	m_IsGraphValid = false;

}

void GridComponent::InsertAndParent(dae::GameObject* object, int row, int column)
{
	object->SetParent(GetOwner(), false, false);
	Insert(object, row, column);
}

glm::vec2 GridComponent::GetPositionAt(int row, int column)
{
	float x = column * m_CellWidth;
	float y = row * m_CellHeight;
	return glm::vec2(x,y);
}

int GridComponent::GetIndexFromPosition(const glm::vec2& pos)
{
	const int row = (int)(std::clamp(pos.y, 0.0f, m_CellHeight - 0.01f) / m_CellHeight);
	const int col = (int)(std::clamp(pos.x, 0.0f, m_CellWidth - 0.01f) / m_CellWidth);
	return (row * m_Columns + col);

}

//Engine::Graph* GridComponent::GetGraphFromGrid()
//{
//    if (!m_IsGraphValid)
//    {
//        m_pCachedGraph = std::make_unique<Engine::Graph>(m_Rows, m_Columns);
//
//        // Create nodes only where a 2x2 area is free
//        for (int row{}; row < m_Rows - 1; ++row) // Note: -1 to prevent out of bounds
//        {
//            for (int col{}; col < m_Columns - 1; ++col) // Note: -1 to prevent out of bounds
//            {
//                // Check if all 4 cells in the 2x2 area are free
//                if (!isStaticAt(row, col) && 
//                    !isStaticAt(row + 1, col) && 
//                    !isStaticAt(row, col + 1) && 
//                    !isStaticAt(row + 1, col + 1))
//                {
//                    m_pCachedGraph->AddNode(row, col, GetPositionAt(row, col));
//                }
//            }
//        }
//
//        // Connect nodes where a 2x2 entity can move between positions
//        for (int row{}; row < m_Rows - 1; ++row)
//        {
//            for (int col{}; col < m_Columns - 1; ++col)
//            {
//                Engine::GraphNode* current = m_pCachedGraph->GetNode(row, col);
//                if (!current) continue;
//
//                // LEFT (check if we can move left while keeping all 2x2 cells free)
//                if (col - 1 >= 0 && m_pCachedGraph->GetNode(row, col - 1) &&
//                    !isStaticAt(row, col - 1) && 
//                    !isStaticAt(row + 1, col - 1))
//                {
//                    auto* left = m_pCachedGraph->GetNode(row, col - 1);
//                    m_pCachedGraph->ConnectNodes(current, left);
//                }
//
//                // RIGHT (check right+1 column is free in both rows)
//                if (col + 2 < m_Columns && m_pCachedGraph->GetNode(row, col + 1) &&
//                    !isStaticAt(row, col + 2) && 
//                    !isStaticAt(row + 1, col + 2))
//                {
//                    auto* right = m_pCachedGraph->GetNode(row, col + 1);
//                    m_pCachedGraph->ConnectNodes(current, right);
//                }
//
//                // UP (check row-1 is free in both columns)
//                if (row - 1 >= 0 && m_pCachedGraph->GetNode(row - 1, col) &&
//                    !isStaticAt(row - 1, col) && 
//                    !isStaticAt(row - 1, col + 1))
//                {
//                    auto* up = m_pCachedGraph->GetNode(row - 1, col);
//                    m_pCachedGraph->ConnectNodes(current, up);
//                }
//
//                // DOWN (check row+2 is free in both columns)
//                if (row + 2 < m_Rows && m_pCachedGraph->GetNode(row + 1, col) &&
//                    !isStaticAt(row + 2, col) && 
//                    !isStaticAt(row + 2, col + 1))
//                {
//                    auto* down = m_pCachedGraph->GetNode(row + 1, col);
//                    m_pCachedGraph->ConnectNodes(current, down);
//                }
//            }
//        }
//        m_IsGraphValid = true;
//    }
//    return m_pCachedGraph.get();
//}

//Engine::Graph* GridComponent::GetGraphFromGrid()
//{
//	if (!m_IsGraphValid)
//	{
//		m_pCachedGraph = std::make_unique<Engine::Graph>(m_Rows, m_Columns);
//
//		for (int row = 0; row < m_Rows; ++row)
//		{
//			for (int col = 0; col < m_Columns; ++col)
//			{
//				m_pCachedGraph->AddNode(row, col, GetPositionAt(row, col));
//			}
//		}
//
//		auto Is2x2Walkable = [&](int row, int col) -> bool {
//			if (row + 1 >= m_Rows || col + 1 >= m_Columns)
//				return false; 
//
//			return !isStaticAt(row, col) &&
//				!isStaticAt(row + 1, col) &&
//				!isStaticAt(row, col + 1) &&
//				!isStaticAt(row + 1, col + 1);
//			};
//
//		for (int row = 0; row < m_Rows; ++row)
//		{
//			for (int col = 0; col < m_Columns; ++col)
//			{
//				if (!Is2x2Walkable(row, col)) continue;
//
//				Engine::GraphNode* current = m_pCachedGraph->GetNode(row, col);
//
//				// LEFT
//				if (col - 1 >= 0 && Is2x2Walkable(row, col - 1))
//				{
//					auto* left = m_pCachedGraph->GetNode(row, col - 1);
//					m_pCachedGraph->ConnectNodes(current, left);
//				}
//
//				// RIGHT
//				if (col + 1 < m_Columns && Is2x2Walkable(row, col + 1))
//				{
//					auto* right = m_pCachedGraph->GetNode(row, col + 1);
//					m_pCachedGraph->ConnectNodes(current, right);
//				}
//
//				// UP
//				if (row - 1 >= 0 && Is2x2Walkable(row - 1, col))
//				{
//					auto* up = m_pCachedGraph->GetNode(row - 1, col);
//					m_pCachedGraph->ConnectNodes(current, up);
//				}
//
//				// DOWN
//				if (row + 1 < m_Rows && Is2x2Walkable(row + 1, col))
//				{
//					auto* down = m_pCachedGraph->GetNode(row + 1, col);
//					m_pCachedGraph->ConnectNodes(current, down);
//				}
//			}
//		}
//
//		m_IsGraphValid = true;
//	}
//
//	return m_pCachedGraph.get();
//}

Engine::Graph* GridComponent::GetGraphFromGrid()
{
	if (!m_IsGraphValid)
	{
		m_pCachedGraph = std::make_unique<Engine::Graph>(m_Rows, m_Columns);

		// Lambda to check if a 2x2 area is walkable
		auto Is2x2Walkable = [&](int row, int col) -> bool {
			if (row + 1 >= m_Rows || col + 1 >= m_Columns)
				return false;
			return !isStaticAt(row, col) &&
				!isStaticAt(row + 1, col) &&
				!isStaticAt(row, col + 1) &&
				!isStaticAt(row + 1, col + 1);
			};

		// Add nodes only for walkable 2x2 regions
		for (int row = 0; row < m_Rows; ++row)
		{
			for (int col = 0; col < m_Columns; ++col)
			{
				if (Is2x2Walkable(row, col))
				{
					m_pCachedGraph->AddNode(row, col, GetPositionAt(row, col));
				}
			}
		}

		// Connect only existing (and thus walkable) nodes
		for (int row = 0; row < m_Rows; ++row)
		{
			for (int col = 0; col < m_Columns; ++col)
			{
				Engine::GraphNode* current = m_pCachedGraph->GetNode(row, col);
				if (!current) continue;

				// LEFT
				if (col - 1 >= 0)
				{
					auto* left = m_pCachedGraph->GetNode(row, col - 1);
					if (left) m_pCachedGraph->ConnectNodes(current, left);
				}

				// RIGHT
				if (col + 1 < m_Columns)
				{
					auto* right = m_pCachedGraph->GetNode(row, col + 1);
					if (right) m_pCachedGraph->ConnectNodes(current, right);
				}

				// UP
				if (row - 1 >= 0)
				{
					auto* up = m_pCachedGraph->GetNode(row - 1, col);
					if (up) m_pCachedGraph->ConnectNodes(current, up);
				}

				// DOWN
				if (row + 1 < m_Rows)
				{
					auto* down = m_pCachedGraph->GetNode(row + 1, col);
					if (down) m_pCachedGraph->ConnectNodes(current, down);
				}
			}
		}

		m_IsGraphValid = true;
	}

	return m_pCachedGraph.get();
}

dae::GameObject* GridComponent::Get(int row, int column)
{
	return m_Grid[row][column];
}

bool GridComponent::isStaticAt(int row, int column)
{
	dae::GameObject* object = Get(row, column);
	if (auto component = object->GetComponent<PhysicsComponent>())
	{
		return (component->GetIsStatic());
	} 
	return false;
}
