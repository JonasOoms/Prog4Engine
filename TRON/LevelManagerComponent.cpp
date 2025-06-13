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
#include "InputManager.h"

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
	LoadLevel("../Data/Levels/LevelOne.json");
	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("LevelIntro"), 20.f);
}

void LevelManagerComponent::EndPlay()
{
	//ClearLevel();
}

void LevelManagerComponent::Update(float)
{
	if (m_ExitLevel)
	{
		ClearLevel();
		m_Game->CurrentScore = m_Score;
		m_Lives = 3;
		m_Score = 0;
		m_ExitLevel = false;
		m_LoadNextLevel = false;
		TRONGame* game = m_Game;
		dae::SceneManager::GetInstance().GetCurrentScene().RemoveAll();
		if (game->GetGameMode() == GameMode::VS)
		{
			game->LoadMainMenu();
		}
		else
		{
			game->LoadScoreScreen();
		}
	}
	else if (m_LoadNextLevel)
	{
		ClearLevel();
		switch (m_SelectedLevel % 3)
		{
		case 0:
			LoadLevel("../Data/Levels/LevelOne.json");
			break;
		case 1:
			LoadLevel("../Data/Levels/LevelTwo.json");
			break;
		case 2:
			LoadLevel("../Data/Levels/LevelThree.json");
		}
		m_LoadNextLevel = false;
	}
}

void LevelManagerComponent::LoadLevel(const std::string& filePath)
{
	JSONLevelLoader loader{};

	switch (m_Game->GetGameMode())
	{
		case GameMode::Singleplayer:
		{
			m_Level = std::move(loader.LoadSinglePlayer(filePath, dae::SceneManager::GetInstance().GetCurrentScene()));
			break;
		}
		case GameMode::Coop:
		{
			m_Level = std::move(loader.LoadCoOp(filePath, dae::SceneManager::GetInstance().GetCurrentScene()));
			break;
		}
		case GameMode::VS:
		{
			m_Level = std::move(loader.LoadVS(filePath, dae::SceneManager::GetInstance().GetCurrentScene()));
			break;
		}
	}

	
	m_Level->m_GridObject->GetComponent<GridComponent>()->GetGraphFromGrid();

	for (dae::GameObject* player : m_Level->m_Players)
	{
		if (PlayerTankHandlerComponent* handler = player->GetComponent<PlayerTankHandlerComponent>())
		{
			handler->GetEventDispatcher()->AddObserver(this);
		}
	}

	size_t enemyAmount{};
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


	dae::InputManager::GetInstance().GetPlayerController(-1)->GetInputMapping()->AddInputBinding(SDLK_F1, TriggerType::Released, std::make_unique<SkipLevelCommand>(this));

	ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("LevelIntro"), 20.f);
}

void LevelManagerComponent::SwitchNextLevel()
{
	++m_SelectedLevel; 
	m_ExitLevel = false; 
	m_LoadNextLevel = true;

}

void LevelManagerComponent::ClearLevel()
{
	if (m_Level)
	{

		m_Level->m_Diamond->Destroy();
		m_Level->m_GridPathRenderer->Destroy();
		m_Level->m_GridObject->Destroy();

		for (dae::GameObject* player : m_Level->m_Players)
		{
			//player->RemoveComponent<PlayerTankHandlerComponent>();
			player->Destroy();
		}

		for (dae::GameObject* enemy : m_Level->m_Enemies)
		{
			enemy->Destroy();
		}

		for (dae::GameObject* recognizer : m_Level->m_Recognisers)
		{
			recognizer->Destroy();
		}

		ServiceLocator::GetPhysicsSystem()->UnregisterAllPhysicsComponents();
		dae::InputManager::GetInstance().ClearAllMappings();

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
				it = std::find(m_Level->m_Recognisers.begin(), m_Level->m_Recognisers.end(), ctx.enemy);
				if (it != m_Level->m_Recognisers.end())
				{
					m_Level->m_Recognisers.erase(it);
					m_Score += 250;
				}
			}

			--m_EnemiesLeft;

			m_ScoreCounter->SetText(std::to_string(m_Score));

			if (m_EnemiesLeft <= 0)
			{
				dae::InputManager::GetInstance().ClearAllMappings();

				ClearLevel();

				++m_SelectedLevel;
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

SkipLevelCommand::SkipLevelCommand(LevelManagerComponent* level):
	m_LevelManager{level}
{
}

void SkipLevelCommand::Execute()
{
	m_LevelManager->SwitchNextLevel();
	//m_LevelManager->ClearLevel();
}
