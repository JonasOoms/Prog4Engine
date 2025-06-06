#pragma once
#include "Component.h"
#include "Level.h"
#include <memory>

class LevelManagerComponent final : public Component
{
public:
	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	

private:
	std::unique_ptr<Level> m_Level;
};

