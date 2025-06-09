#include "ScoreManagerComponent.h"
#include "Minigin.h"
#include "ServiceLocator.h"
#include "GameRegistries.h"

ScoreManagerComponent::ScoreManagerComponent(TRONGame* game):
	m_Game{game},
	m_State{std::make_unique<SubmitOrQuitState>()},
	m_ScoreGUIFont{dae::ResourceManager::GetInstance().LoadFont("Fonts/Volter_Goldfish.ttf", 72)}
{
}

void ScoreManagerComponent::Interact(ScoreManagerAction action)
{
	if (auto newState = m_State->Interact(*this, action))
	{
		m_State = std::move(newState);
	}
}

void ScoreManagerComponent::Render()
{
	if (m_State)
	{
		m_State->Render(*this);
	}
}

TRONGame* ScoreManagerComponent::GetGame()
{
	return m_Game;
}




std::unique_ptr<ScoreGUIState> SubmitOrQuitState::Interact(ScoreManagerComponent& scoreManager, ScoreManagerAction action)
{
	switch (action)
	{
		case ScoreManagerAction::Back:
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			scoreManager.GetGame()->LoadMainMenu();
			break;
		}
		case ScoreManagerAction::Enter:
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Select"), 10);
			return std::make_unique<SubmitScoreState>();
			break;
		}
	}
	return nullptr;
}

void SubmitOrQuitState::Render(ScoreManagerComponent& scoreManager)
{
	dae::Font* font = scoreManager.GetFont();
	dae::Renderer& renderer = dae::Renderer::GetInstance();

	const int screenMiddle = dae::Minigin::windowWidth / 2;

	
	renderer.DrawString("YOU DIED", font, glm::vec2{ screenMiddle - 50 , 15}, SDL_Color{ 0,0,255,255 }, 100, 40);
	renderer.DrawString("YOUR SCORE: " + std::to_string(scoreManager.GetGame()->CurrentScore), font, glm::vec2{screenMiddle - 200/2 , 80}, SDL_Color{0,0,255,255}, 200, 50);

	renderer.DrawString("HIGHSCORES", font, glm::vec2{ screenMiddle - 300 / 2 , 140 }, SDL_Color{ 0,255,255,255 }, 300, 70);

	const HighScoreBuffer& HighScoreBuffer = scoreManager.GetGame()->GetHighScoreBuffer();
	const auto entries = HighScoreBuffer.Get();
	int numEntriesToRender = std::min(static_cast<int>(HighScoreBuffer.count()), 10);
	
	const int startY = 220;
	const int lineSpacing = 40;

	for (int i = 0; i < numEntriesToRender; ++i)
	{
		const auto& entry = entries[i];
		std::string name = std::to_string(i + 1) + ". " + entry.first;
		if (i == 0)
		{
			name = "¥. " + entry.first;
		}
		std::string score = std::to_string(entry.second);

	
		renderer.DrawString(name, font, glm::vec2{ screenMiddle - 120, startY + i * lineSpacing }, SDL_Color{ 255,255,255,255 }, 100, 30);
		renderer.DrawString(score, font, glm::vec2{ screenMiddle + 80, startY + i * lineSpacing }, SDL_Color{ 255,255,255,255 }, 50, 30);
	}




	renderer.DrawString("A to submit score", font, glm::vec2{5, dae::Minigin::windowHeight - 40}, SDL_Color{255,255,255,255}, 200, 30);
	renderer.DrawString("B to return to gamemode select", font, glm::vec2{ dae::Minigin::windowWidth - 370, dae::Minigin::windowHeight - 40 }, SDL_Color{ 255,255,255,255 }, 300, 30);
}

std::unique_ptr<ScoreGUIState> SubmitScoreState::Interact(ScoreManagerComponent& scoreManager, ScoreManagerAction action)
{
	switch (action)
	{
		case (ScoreManagerAction::Back):
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			return std::make_unique<SubmitOrQuitState>();
			break;
		}
		case (ScoreManagerAction::Enter):
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Save"), 10);
			
			std::string nameString;
			for (int i = 0; i < 3; ++i)
			{
				nameString += m_Name[i];
			}
			int score = scoreManager.GetGame()->CurrentScore;
			scoreManager.GetGame()->PutScore(nameString, score);

			scoreManager.GetGame()->LoadMainMenu();
			break;
		}
		case (ScoreManagerAction::Left):
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			m_SelectedChar = std::abs(m_SelectedChar - 1) % 3;
			break;
		}
		case (ScoreManagerAction::Right):
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			m_SelectedChar = std::abs(m_SelectedChar + 1) % 3;
			break;
		}
		case ScoreManagerAction::Up:
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			int letterIndex = m_Name[m_SelectedChar] - 'A';
			letterIndex = (letterIndex + 1) % 26;
			m_Name[m_SelectedChar] = static_cast<char>('A' + letterIndex);
			break;
		}
		case ScoreManagerAction::Down:
		{
			ServiceLocator::GetSoundSystem()->Play(TRONRegistries::GameSoundRegistry.Get("Back"), 10);
			int letterIndex = m_Name[m_SelectedChar] - 'A';
			letterIndex = (letterIndex - 1 + 26) % 26;
			m_Name[m_SelectedChar] = static_cast<char>('A' + letterIndex);
			break;
		}
	}
	return nullptr;
}

void SubmitScoreState::Render(ScoreManagerComponent& scoreManager)
{
	dae::Font* font = scoreManager.GetFont();
	dae::Renderer& renderer = dae::Renderer::GetInstance();



	const int screenMiddle = dae::Minigin::windowWidth / 2;
	renderer.DrawString("SUBMIT SCORE", font, glm::vec2{ screenMiddle - 200/2 , 65 }, SDL_Color{ 0,255,0,255 }, 200, 60);

	std::string nameString;
	for (int i = 0; i < 3; ++i)
	{
		nameString += m_Name[i];
		if (i != 2) nameString += ' ';
	}

	const int nameY = 200;
	const int letterSpacing = 50; 
	const int totalWidth = 3 * letterSpacing;
	int startX = screenMiddle - totalWidth / 2;

	for (int i = 0; i < 3; ++i)
	{
		std::string letterString(1, m_Name[i]);
		renderer.DrawString(letterString, font, glm::vec2{ static_cast<float>(startX + i * letterSpacing), static_cast<float>(nameY) }, SDL_Color{0,255,0}, 50, 50);
	}

	
	int markerX = startX + m_SelectedChar * letterSpacing + 15; 
	int markerY = nameY + 55;

	renderer.DrawString("^", font, glm::vec2{ static_cast<float>(markerX), static_cast<float>(markerY) }, SDL_Color{0,255,0,255}, 20, 20);



	renderer.DrawString("A to submit", font, glm::vec2{ 10, static_cast<float>(dae::Minigin::windowHeight - 40) }, SDL_Color{255,255,255,255}, 150, 30);
	renderer.DrawString("B to go back", font, glm::vec2{ static_cast<float>(dae::Minigin::windowWidth - 170), static_cast<float>(dae::Minigin::windowHeight - 40) }, SDL_Color{255,255,255,255}, 150, 30);
}


ScoreManagerCommand::ScoreManagerCommand(ScoreManagerComponent* scoreManager, ScoreManagerAction action):
	m_Binding{scoreManager},
	m_Action{action}
{
}

void ScoreManagerCommand::Execute()
{
	m_Binding->Interact(m_Action);
}

