#pragma once
#include "EventDispatcher.h"
/// <summary>/// 
// Inheriting from this interface means the component is an enemy handler, used for HP and game state
/// </summary>
class IEnemyHandler
{
public:
	virtual ~IEnemyHandler() = default;

	virtual void Hurt(int damage) = 0;
	virtual int GetHealth() = 0;
	virtual EventDispatcher* GetEnemyEventDispatcher() = 0;

};