#pragma once
#include "Component.h"
#include "EventDispatcher.h"

class PlayerTankHandlerComponent : public Component
{
public:
	PlayerTankHandlerComponent();
	void Hurt();
	EventDispatcher* GetEventDispatcher();
private:
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
};

