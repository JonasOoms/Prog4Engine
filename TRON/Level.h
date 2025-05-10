#pragma once
#include "GameObject.h"

struct Level
{
public:
	dae::GameObject* m_Player;
	dae::GameObject* m_GridObject;
	std::vector<dae::GameObject*> m_CollisionTiles;
	std::vector<dae::GameObject*> m_Enemies;
	std::vector<dae::GameObject*> m_Recognisers;
};

