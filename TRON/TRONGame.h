#pragma once
#include <IGame.h>
#include "LevelLoader.h"


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
	void LoadLevel();

	// Inherited via IGame
	void Load() override;
	std::string_view GetGameTitle() override;

	void SetIsInLevel(bool isInLevel) {m_IsInLevel = isInLevel;}
	bool GetIsInLevel() const { return m_IsInLevel; }
	void ChangeGameMode(GameMode gamemode);
	GameMode GetGameMode() const { return m_SelectedGameMode; }

private:

	bool m_IsInLevel{};
	GameMode m_SelectedGameMode{};
	

};

