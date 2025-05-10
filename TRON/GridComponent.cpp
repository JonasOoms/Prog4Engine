#include "GridComponent.h"
#include "GameObject.h"
#include "Event.h"

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

dae::GameObject* GridComponent::Get(int row, int column)
{
	return m_Grid[row][column];
}
