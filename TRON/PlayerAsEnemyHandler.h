#pragma once
#include "Component.h"
#include "IEnemy.h"
#include "ShootCommand.h"
#include "Observer.h"
#include "GameEnums.h"

class PlayerAsEnemyHandler : public Component, public IEnemyHandler, public Observer
{
public:
	PlayerAsEnemyHandler(EnemyShootCommand* enemyShoot);
	virtual void ComponentOwnerInitialized() override;
	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;
	// Inherited via IEnemyHandler
	void Hurt(int damage) override;

	int GetHealth() override;

	EventDispatcher* GetEnemyEventDispatcher() override;
private:
	EnemyShootCommand* m_pShootCommand{};
	TRONEnums::Direction m_LastDirection{TRONEnums::Direction::Right};
	std::unique_ptr<EventDispatcher> m_EventDispatcher;


};

