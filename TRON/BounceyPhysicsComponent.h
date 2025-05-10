#pragma once
#include "PhysicsComponent.h"
#include "GameObject.h"

class BulletState;
class FlyingState;

class BounceyPhysicsComponent final : public PhysicsComponent
{
public:
	BounceyPhysicsComponent(glm::vec2 size, glm::vec2 velocity, dae::GameObject* tankFiredFrom);

	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
	virtual void Update(float deltaTime) override;

	dae::GameObject* GetTankFiredFrom() { return m_TankFiredFrom; }
	BulletState* GetState() { return m_State.get(); }
	void SetState(BulletState* state);

private:
	std::unique_ptr<BulletState> m_State;
	dae::GameObject* m_TankFiredFrom;
};

class BulletState
{
public:
	virtual ~BulletState() = default;
	virtual void Update(BounceyPhysicsComponent& bullet, float dt) = 0;
	virtual void OnCollision(BounceyPhysicsComponent& bullet, float , PhysicsComponent&, dae::GameObject& gameObject , const HitInfo&) = 0;
};

class FlyingState final : public BulletState
{
public:
	FlyingState() = default;
	void Update(BounceyPhysicsComponent& bullet, float dt) override;
	void OnCollision(BounceyPhysicsComponent& bullet, float dt, PhysicsComponent& component, dae::GameObject& gameObject, const HitInfo& hitInfo) override;
private:
	bool m_HasBounced{ false };
	int m_countCollisions{};
};

class ExplodingState final : public BulletState
{
public:
	ExplodingState() = default;
	void Update(BounceyPhysicsComponent& bullet, float dt) override;
	void OnCollision(BounceyPhysicsComponent& , float, PhysicsComponent&, dae::GameObject&, const HitInfo& ) override {};
private:
	float m_ExplosionCountdown{ 5.0f };
};