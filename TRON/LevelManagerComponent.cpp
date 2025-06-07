#include "LevelManagerComponent.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include "GridComponent.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"
#include "IEnemy.h"

void LevelManagerComponent::BeginPlay()
{
	JSONLevelLoader loader{};
	m_Level = std::move(loader.Parse("../Data/Levels/LevelUno.json", dae::SceneManager::GetInstance().GetCurrentScene()));
	m_Level->m_GridObject->GetComponent<GridComponent>()->GetGraphFromGrid();

	int enemyAmount{};
	// bind enemy tanks and recognisers to the Level Manager 
	for (dae::GameObject* enemy : m_Level->m_Enemies)
	{
		if (IEnemyHandler* handler = enemy->GetComponent<IEnemyHandler>())
		{
			handler->GetEnemyEventDispatcher()->AddObserver(this);
			++enemyAmount;
		}
	}

	for (dae::GameObject* enemy : m_Level->m_Recognisers)
	{
		if (IEnemyHandler* handler = enemy->GetComponent<IEnemyHandler>())
		{
			handler->GetEnemyEventDispatcher()->AddObserver(this);
			++enemyAmount;
		}
	}


	std::cout << enemyAmount << std::endl;

	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("LevelIntro"), 40.f);
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

void LevelManagerComponent::Notify(const Event& , EventDispatcher* )
{
}
