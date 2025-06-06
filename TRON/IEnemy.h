#pragma once

/// <summary>/// 
// Inheriting from this interface means the component is an enemy handler, used for HP and game state
/// </summary>
class IEnemyHandlerComponent
{
public:
	virtual void Hurt(int damage) = 0;
	virtual int GetHealth() = 0;

};