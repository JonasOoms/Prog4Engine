#pragma once
#include "Component.h"

class GameModeSelectorLoadingComponent : public Component
{
public:
	virtual void EndPlay() override;
	virtual void BeginPlay() override;
private:
	dae::GameObject* m_Selector;
};

