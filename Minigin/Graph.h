#pragma once
#include <vector>
#include <glm.hpp>
#include <unordered_map>
#include <queue>
#include <map>

namespace Engine
{
	/// <summary>
	/// These graphs are grid based. Supposed to be for use in grid based games, but can be repurposed for other uses
	/// </summary>
	struct GraphNode
	{
		int row{}, col{};
		glm::vec2 worldPosition;
		std::vector<GraphNode*> connections;
	};

	class Graph
	{
	public:
		Graph(int row, int columns);

		GraphNode* GetNode(int nodeIndex);
		GraphNode* GetNode(int row, int col);
		GraphNode* GetClosestNode(const glm::vec2& position);
		GraphNode* GetRandomNode();

		GraphNode* AddNode(int row, int col, glm::vec2 worldPosition);

		void ConnectNodes(GraphNode* a, GraphNode* b);

		std::vector<GraphNode> GetNodes() const;


	private:
		int m_Rows{};
		int m_Cols{};
		std::unordered_map<int, GraphNode> m_NodeMap;



		int GetIndex(int row, int col) const;

	};

	std::vector<Engine::GraphNode*> BFSPathSearch(Engine::GraphNode* start, Engine::GraphNode* end);

}