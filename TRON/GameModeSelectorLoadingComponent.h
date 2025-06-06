#pragma once
#include "Component.h"
#include "TRONGame.h"

class GameModeSelectorLoadingComponent : public Component
{
public:

	GameModeSelectorLoadingComponent(TRONGame* game);

	virtual void EndPlay() override;
	virtual void BeginPlay() override;
private:
	TRONGame* m_Game{};
	dae::GameObject* m_Selector{};
};

