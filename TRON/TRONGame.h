#pragma once
#include <IGame.h>
#include "LevelLoader.h"

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
private:


	

};

