#pragma once
#include <glm.hpp>
#include "Component.h"
#include "GameObject.h"
#include "EngineStructs.h"
#include <iostream>

struct HitInfo;

class PhysicsComponent : public Component
{
public:

	PhysicsComponent(const glm::vec2& size);
	~PhysicsComponent() override;

	virtual void BeginPlay() override;
	virtual void EndPlay() override;
	virtual void ComponentOwnerInitialized() override;

	/// <summary>
	/// Handle collision with other physics components. DO NOT CALL THIS DIRECTLY UNLESS YOU KNOW WHAT YOU ARE DOING
	/// </summary>
	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&);

	Engine::Rect GetBoundingBox() const;
	glm::vec2 GetPosition() const;
	float GetWidth() const { return m_Bounds.x; }
	float GetHeight() const { return m_Bounds.y; }
	void SetIsStatic(bool value) { this->isStatic = value; };
	bool GetIsStatic() { return this->isStatic; };

	void SetVelocity(glm::vec2 velocity) { m_Velocity = velocity; }
	glm::vec2 GetVelocity() { return m_Velocity; }
	
	bool HasCollided{false};
protected:
	glm::vec2 m_Velocity{ 0,0 };
private:
	bool isStatic{ false };
	bool doDebugRendering{ false };
	glm::vec2 m_Bounds{ 0,0 };
};

