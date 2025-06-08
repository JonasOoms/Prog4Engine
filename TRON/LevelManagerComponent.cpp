#include "LevelManagerComponent.h"
#include "LevelLoader.h"
#include "SceneManager.h"
#include "GridComponent.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"
#include "GameEvents.h"
#include "IEnemy.h"
#include "InputManager.h"
#include "PlayerHandlerComponent.h"

LevelManagerComponent::LevelManagerComponent(TRONGame* game, TextRenderComponent* scoreCounter, TextRenderComponent* lifeCounter, TextRenderComponent* highScoreCounter):
	m_Game{game},
	m_LifeCounter{lifeCounter},
	m_ScoreCounter{scoreCounter},
	m_HighScoreCounter{highScoreCounter}
{
}

void LevelManagerComponent::BeginPlay()
{
	if (m_Level)
	{
		ClearLevel();
	}
	LoadLevel("../Data/Levels/LevelUno.json");
	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("LevelIntro"), 40.f);
}

void LevelManagerComponent::EndPlay()
{
	ClearLevel();
}

void LevelManagerComponent::Update(float)
{
	if (m_ExitLevel)
	{
		m_Game->SetIsInLevel(false);
		m_Lives = 3;
		m_Score = 0;
		m_ExitLevel = false;
		m_LoadNextLevel = true;
		dae::SceneManager::GetInstance().GetCurrentScene().RemoveAll();
		dae::SceneManager::GetInstance().SelectScene("Menu");
	}
	else if (m_LoadNextLevel)
	{
		
		LoadLevel("../Data/Levels/LevelTwo.json");
		m_LoadNextLevel = false;
	}
}

void LevelManagerComponent::LoadLevel(const std::string& filePath)
{
	JSONLevelLoader loader{};
	m_Level = std::move(loader.Parse(filePath, dae::SceneManager::GetInstance().GetCurrentScene()));
	m_Level->m_GridObject->GetComponent<GridComponent>()->GetGraphFromGrid();

	for (dae::GameObject* player : m_Level->m_Players)
	{
		if (PlayerTankHandlerComponent* handler = player->GetComponent<PlayerTankHandlerComponent>())
		{
			handler->GetEventDispatcher()->AddObserver(this);
		}
	}

	uint8_t enemyAmount{};
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

	m_EnemiesLeft = enemyAmount;

	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("LevelIntro"), 40.f);
}

void LevelManagerComponent::ClearLevel()
{
	if (m_Level)
	{
		m_Level->m_GridPathRenderer->Destroy();
		m_Level->m_GridObject->Destroy();

		for (dae::GameObject* enemy : m_Level->m_Enemies)
		{
			enemy->Destroy();
		}

		for (dae::GameObject* player : m_Level->m_Players)
		{
			player->Destroy();
		}

		for (dae::GameObject* recognizer : m_Level->m_Recognisers)
		{
			recognizer->Destroy();
		}

	}
}

void LevelManagerComponent::Notify(const Event& event, EventDispatcher* )
{
	switch (event.eventType)
	{
		case TRONEvents::EVENT_ENEMY_DEATH:
		{
			TRONEvents::EventContexts::EnemyDeathEventContext ctx = std::any_cast<TRONEvents::EventContexts::EnemyDeathEventContext>(event.eventContext);

			auto it = std::find(m_Level->m_Enemies.begin(), m_Level->m_Enemies.end(), ctx.enemy);

			if (it != m_Level->m_Enemies.end())
			{
				m_Level->m_Enemies.erase(it);
				m_Score += 100;
			}
			else
			{
				m_Level->m_Recognisers.erase(std::find(m_Level->m_Recognisers.begin(), m_Level->m_Recognisers.end(), ctx.enemy));
				m_Score += 250;
			}

			--m_EnemiesLeft;

			m_ScoreCounter->SetText(std::to_string(m_Score));

			if (m_EnemiesLeft <= 0)
			{
				dae::InputManager::GetInstance().ClearAllMappings();

				ClearLevel();

				m_LoadNextLevel = true;
			}
			break;
		}
		case TRONEvents::EVENT_PLAYER_DEATH:
		{
			--m_Lives;
			
			switch (m_Lives)
			{
				case 3:
				{
					m_LifeCounter->SetText("1st");
					break;
				}
				case 2:
				{
					m_LifeCounter->SetText("2nd");
					break;
				}
				case 1:
				{
					m_LifeCounter->SetText("3rd");
					break;
				}

			}

			if (m_Lives <= 0)
			{
				// TODO Change to end screen
				ClearLevel();
				m_ExitLevel = true;
				break;
			}

			ClearLevel();
			m_LoadNextLevel = true;
			break;
		}
	}



}
