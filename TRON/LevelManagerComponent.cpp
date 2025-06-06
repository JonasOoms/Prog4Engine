#include "LevelManagerComponent.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include "GridComponent.h"

void LevelManagerComponent::BeginPlay()
{
	JSONLevelLoader loader{};
	m_Level = std::move(loader.Parse("../Data/Levels/LevelUno.json", dae::SceneManager::GetInstance().GetCurrentScene()));
	m_Level->m_GridObject->GetComponent<GridComponent>()->GetGraphFromGrid();
}

void LevelManagerComponent::EndPlay()
{
	if (m_Level)
	{
		m_Level->m_GridObject->Destroy();

		for (dae::GameObject* enemy : m_Level->m_Enemies)
		{
			enemy->Destroy();
		}

		for (dae::GameObject* player : m_Level->m_Players)
		{
			player->Destroy();
		}

		for (dae::GameObject* recognizer : m_Level->m_Enemies)
		{
			recognizer->Destroy();
		}

	}
}
