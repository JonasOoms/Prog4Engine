#pragma once
#include "GameObject.h"

struct Level
{
public:
	std::vector<dae::GameObject*> m_Players;
	dae::GameObject* m_GridObject;
	dae::GameObject* m_GridPathRenderer;
	dae::GameObject* m_Diamond;
	std::vector<dae::GameObject*> m_CollisionTiles;
	std::vector<dae::GameObject*> m_Enemies;
	std::vector<dae::GameObject*> m_Recognisers;
};

