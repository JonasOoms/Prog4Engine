#pragma once
#include "PhysicsComponent.h"
#include "GameObject.h"

class BulletState;
class FlyingState;

class BounceyPhysicsComponent final : public PhysicsComponent
{
public:
	BounceyPhysicsComponent(const glm::vec2& size, const glm::vec2& velocity, dae::GameObject* tankFiredFrom); 

	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
	virtual void Update(float deltaTime) override;

	dae::GameObject* GetTankFiredFrom() { return m_TankFiredFrom; }
	BulletState* GetState() { return m_State.get(); }

private:
	std::unique_ptr<BulletState> m_State;
	dae::GameObject* m_TankFiredFrom;
};

class BulletState
{
public:
	virtual ~BulletState() = default;
	virtual std::unique_ptr<BulletState> Update(BounceyPhysicsComponent& bullet, float dt) = 0;
	virtual std::unique_ptr<BulletState> OnCollision(BounceyPhysicsComponent& bullet, float , PhysicsComponent&, dae::GameObject& gameObject , const HitInfo&) = 0;
};

class FlyingState final : public BulletState
{
public:
	FlyingState() = default;
	std::unique_ptr<BulletState> Update(BounceyPhysicsComponent& bullet, float dt) override;
	std::unique_ptr<BulletState> OnCollision(BounceyPhysicsComponent& bullet, float dt, PhysicsComponent& component, dae::GameObject& gameObject, const HitInfo& hitInfo) override;
private:
	bool m_HasBounced{ false };
	int m_countCollisions{};
};

class ExplodingState final : public BulletState
{
public:
	ExplodingState() = default;
	std::unique_ptr<BulletState> Update(BounceyPhysicsComponent& bullet, float dt) override;
	std::unique_ptr<BulletState> OnCollision(BounceyPhysicsComponent&, float, PhysicsComponent&, dae::GameObject&, const HitInfo&) override { return nullptr; };
private:
	float m_ExplosionCountdown{ .1f };
};