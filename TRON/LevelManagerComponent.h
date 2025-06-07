#pragma once
#include "Component.h"
#include "Level.h"
#include "Observer.h"
#include <memory>

class LevelManagerComponent final : public Component, public Observer
{
public:
	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	

private:
	std::unique_ptr<Level> m_Level;

	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;
};

