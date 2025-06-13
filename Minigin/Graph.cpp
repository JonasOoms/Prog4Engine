#include "Graph.h"
using namespace Engine;

Engine::Graph::Graph(int row, int columns) :
    m_Rows{ row },
    m_Cols{ columns }
{
}

GraphNode* Engine::Graph::GetNode(int nodeIndex)
{
    return &m_NodeMap.find(nodeIndex)->second;
}

GraphNode* Engine::Graph::GetNode(int row, int col)
{
    int idx = GetIndex(row, col);
    auto it = m_NodeMap.find(idx);
    if (it != m_NodeMap.end())
        return &it->second;
    return nullptr;
}

 GraphNode* Engine::Graph::GetClosestNode(const glm::vec2& position)
{
    if (m_NodeMap.empty()) return nullptr;

    Engine::GraphNode* closest{};
    float closestDistanceSqr = std::numeric_limits<float>::max();

    for (auto& [index, node] : m_NodeMap)
    {

        float distanceSqr = glm::distance(node.worldPosition, position);
        if (distanceSqr < closestDistanceSqr)
        {
            closestDistanceSqr = distanceSqr;
            closest = &node;
        }
    }

    return closest;
}

 GraphNode* Engine::Graph::GetRandomNode()
 {
     if (m_NodeMap.empty()) return nullptr;

     int randomIndex = std::rand() % m_NodeMap.size();
     auto it = m_NodeMap.begin();
     std::advance(it, randomIndex);

     return &(it->second);
 }

GraphNode* Engine::Graph::AddNode(int row, int col, glm::vec2 worldPosition)
{


    int idx = GetIndex(row, col);
    auto [it, inserted] = m_NodeMap.emplace(idx, GraphNode(row, col, worldPosition));
    return &it->second;
}

void Engine::Graph::ConnectNodes(GraphNode* a, GraphNode* b)
{
    if (!a || !b || a == b)
        return;

    auto alreadyConnected = [](GraphNode* from, GraphNode* to) {
        return std::find(from->connections.begin(), from->connections.end(), to) != from->connections.end();
        };

    if (!alreadyConnected(a, b))
        a->connections.emplace_back(b);

    if (!alreadyConnected(b, a))
        b->connections.emplace_back(a);
}

std::vector<GraphNode> Engine::Graph::GetNodes() const
{
    std::vector<GraphNode> result;
    result.reserve(m_NodeMap.size()); 

    for (auto& [key, node] : m_NodeMap)
    {
        result.emplace_back(node);
    }

    return result;
}

int Engine::Graph::GetIndex(int row, int col) const
{
 
    return row * m_Cols + col;
}

std::vector<Engine::GraphNode*> Engine::BFSPathSearch(Engine::GraphNode* start, Engine::GraphNode* end)
{
    std::queue<Engine::GraphNode*> openList = {};
    std::map<Engine::GraphNode*, Engine::GraphNode*> closedList = {};

    openList.push(start);

    while (!openList.empty())
    {
        Engine::GraphNode* const pCurrentNode = openList.front();
        openList.pop();

        if (pCurrentNode == end) break;

        for (Engine::GraphNode* const pConnection : pCurrentNode->connections)
        {

            if (closedList.find(pConnection) == closedList.end())
            {
                openList.push(pConnection);
                closedList[pConnection] = pCurrentNode;
            }

        }

    }

    std::vector<Engine::GraphNode*> path{};
    Engine::GraphNode* pCurrentNode = end;

    while (pCurrentNode != start)
    {
        path.push_back(pCurrentNode);
        pCurrentNode = closedList.at(pCurrentNode);

    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}
