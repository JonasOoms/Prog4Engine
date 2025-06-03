#pragma once
#include "Observer.h"
#include "Component.h"
#include "Graph.h"
#include <vector>
#include <glm.hpp>


namespace dae
{
	class GameObject;
}

/// <summary>
/// Once added to a component, organizes all childrens positions neatly in the grid. Expects no children
/// of the parent to be destroyed or removed out of order.
/// </summary>

class GridComponent final: public Component, public Observer
{
public:
	GridComponent(float width, float height, int rows, int columns);
	virtual void ComponentOwnerInitialized() override;
	void Notify(const Event& event, EventDispatcher* subject) override;
	void InsertAndParent(dae::GameObject* object, int row, int column);
	glm::vec2 GetPositionAt(int row, int column);
	int GetIndexFromPosition(const glm::vec2& pos);

	Engine::Graph* GetGraphFromGrid();

	int GetNumRows() const { return m_Rows; }
	int GetNumColumns() const { return m_Columns; };
	float GetCellWidth() const { return m_CellWidth; }
	float GetCellHeight() const { return m_CellHeight; }

private:
	const float m_Width;
	const float m_Height;
	const int m_Rows;
	const int m_Columns;
	const float m_CellWidth;
	const float m_CellHeight;

	int m_lastAddedIndex{};
	std::vector<std::vector<dae::GameObject*>> m_Grid;

	bool m_IsGraphValid{false};
	std::unique_ptr<Engine::Graph> m_pCachedGraph;


	void Add(dae::GameObject* object);
	void Remove(dae::GameObject* object);
	void Insert(dae::GameObject* object, int row, int column);
	dae::GameObject* Get(int row, int column);

	bool isStaticAt(int row, int column);
	// Inherited via Observer
};
