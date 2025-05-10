#pragma once
#include "Observer.h"
#include "Component.h"
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
private:
	const float m_Width;
	const float m_Height;
	const int m_Rows;
	const int m_Columns;
	const float m_CellWidth;
	const float m_CellHeight;

	int m_lastAddedIndex{};
	std::vector<std::vector<dae::GameObject*>> m_Grid;
	
	void Add(dae::GameObject* object);
	void Remove(dae::GameObject* object);
	void Insert(dae::GameObject* object, int row, int column);
	dae::GameObject* Get(int row, int column);


	// Inherited via Observer
};

