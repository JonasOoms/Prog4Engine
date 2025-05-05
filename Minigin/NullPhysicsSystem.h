#pragma once
#include "PhysicsSystem.h"
class NullPhysicsSystem : public PhysicsSystem
{
public:
	~NullPhysicsSystem() = default;
	void PhysicsUpdate(float) override {};
	void RegisterPhysicsComponent(PhysicsComponent*) override {};
	void UnregisterPhysicsComponent(PhysicsComponent*) override {};
};

