#include "PlayerAsEnemyHandler.h"
#include "GameObject.h"
#include "EngineUtils.h"
#include "GameEvents.h"
#include <iostream>

PlayerAsEnemyHandler::PlayerAsEnemyHandler(EnemyShootCommand* enemyShoot):
	m_pShootCommand{enemyShoot},
	m_EventDispatcher{std::make_unique<EventDispatcher>()}
{
}

void PlayerAsEnemyHandler::ComponentOwnerInitialized()
{
	GetOwner()->GetGameObjectEventDispatcher()->AddObserver(this);
}

void PlayerAsEnemyHandler::Notify(const Event& event, EventDispatcher* )
{
	switch (event.eventType)
	{
		case EngineEvents::EVENT_GAMEOBJECT_TRANSFORMCHANGED:
		{
			auto context = std::any_cast<EventContext::GameObjectTransformChangedEventContext>(event.eventContext);
			
			auto delta = context.newTransform.GetWorldPosition() - context.oldTransform.GetWorldPosition();

			if (glm::length(delta) > 0.1f)
			{
				auto cardinalShootingDirection = Engine::GetClosestCardinal(delta);
				m_pShootCommand->SetDirection(cardinalShootingDirection);
			}
		}
	}
}

void PlayerAsEnemyHandler::Hurt(int)
{
	m_EventDispatcher->NotifyObservers(Event{ TRONEvents::EVENT_PLAYER_DEATH });
}

int PlayerAsEnemyHandler::GetHealth()
{
	return 1;
}

EventDispatcher* PlayerAsEnemyHandler::GetEnemyEventDispatcher()
{
	return m_EventDispatcher.get();
}
