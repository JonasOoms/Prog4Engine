#include "GridPathRenderer.h"
#include "Graph.h"
#include "Renderer.h"
#include "GridComponent.h"

GridPathRenderComponent::GridPathRenderComponent(GridComponent* gridComponent):
	RenderComponent(),
	m_pGrid{gridComponent}
{
}

void GridPathRenderComponent::Render()
{
	Engine::Graph* graph = m_pGrid->GetGraphFromGrid();
	dae::Renderer& renderer = dae::Renderer::GetInstance();

	float cellWidth = m_pGrid->GetCellWidth();
	float cellHeight = m_pGrid->GetCellHeight();

	for (const Engine::GraphNode& node : graph->GetNodes())
	{
		for (auto connection : node.connections)
		{
			renderer.drawLine(static_cast<int>(node.worldPosition.x + cellWidth), static_cast<int>(node.worldPosition.y + cellHeight), static_cast<int>(connection->worldPosition.x + cellWidth), static_cast<int>(connection->worldPosition.y + cellHeight), SDL_Color{28,20,105,255});
		}
	}

}
