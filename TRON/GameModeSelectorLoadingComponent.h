#pragma once
#include "Component.h"
#include "TRONGame.h"

class GameModeSelectorLoadingComponent : public Component
{
public:

	GameModeSelectorLoadingComponent(TRONGame* game, dae::GameObject* selector, dae::GameObject* singlePlayerActivationBox);

	void Update(float);
	virtual void EndPlay() override;
	virtual void BeginPlay() override;
private:

	dae::GameObject* m_SinglePlayerActivationBox{};
	bool m_GotoLevel{};
	TRONGame* m_Game{};
	dae::GameObject* m_Selector{};
};

