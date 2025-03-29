#pragma once
#include "Observer.h"
#include "Component.h"

class AchievementObserver : public Observer, public Component
{
	void Notify(const Event& event, EventDispatcher* subject) override;
};

