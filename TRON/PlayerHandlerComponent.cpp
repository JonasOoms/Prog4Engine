#include "PlayerHandlerComponent.h"
#include "GameEvents.h"

PlayerTankHandlerComponent::PlayerTankHandlerComponent():
	m_EventDispatcher{std::make_unique<EventDispatcher>()}
{
}

void PlayerTankHandlerComponent::Hurt()
{
	m_EventDispatcher->NotifyObservers(Event{ TRONEvents::EVENT_PLAYER_DEATH });
}

EventDispatcher* PlayerTankHandlerComponent::GetEventDispatcher()
{
	return m_EventDispatcher.get();
}
