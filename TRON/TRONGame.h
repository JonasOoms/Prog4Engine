#pragma once
#include <IGame.h>
#include "LevelLoader.h"
#include "HighScoreBuffer.h"


enum class GameMode
{
	Singleplayer,
	Coop,
	VS
};

class TRONGame : public IGame
{
public:
	TRONGame();
	~TRONGame();

	void LoadMainMenu();
	void LoadLevel(GameMode gamemode);
	void LoadScoreScreen();

	// Inherited via IGame
	void Load() override;
	std::string_view GetGameTitle() override;

	void SetIsInLevel(bool isInLevel) {m_IsInLevel = isInLevel;}
	bool GetIsInLevel() const { return m_IsInLevel; }
	void ChangeGameMode(GameMode gamemode);
	GameMode GetGameMode() const { return m_SelectedGameMode; }

	int CurrentScore{};

	void PutScore(const std::string& name, int score) { m_HighScores.Put(name, score); }
	const HighScoreBuffer& GetHighScoreBuffer() { return m_HighScores; }

private:
	std::function<void()> LoadSingleplayerLevel = [this] {
		LoadLevel(GameMode::Singleplayer);
		};

	std::function<void()> LoadCoopLevel = [this] {
		LoadLevel(GameMode::Coop);
		};

	
	bool m_IsInLevel{};
	GameMode m_SelectedGameMode{};
	HighScoreBuffer m_HighScores;


};

