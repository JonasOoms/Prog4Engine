#pragma once
#include "Component.h"
#include "TRONGame.h"
#include "ActivationBoxComponent.h"

class GameModeSelectorLoadingComponent : public Component
{
public:

	GameModeSelectorLoadingComponent(TRONGame* game, dae::GameObject* selector, const std::vector<ActivationBoxComponent*>& activationBoxes);

	void Update(float);
	virtual void EndPlay() override;
	virtual void BeginPlay() override;
private:

	bool m_GotoLevel{};
	TRONGame* m_Game{};
	std::vector<ActivationBoxComponent*> m_ActivationBoxes;
	dae::GameObject* m_Selector{};
};

