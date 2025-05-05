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


	virtual void ComponentOwnerInitialized() override;

	/// <summary>
	/// Handle collision with other physics components. DO NOT CALL THIS DIRECTLY UNLESS YOU KNOW WHAT YOU ARE DOING
	/// </summary>
	virtual void OnCollide(float dt, const PhysicsComponent&, const dae::GameObject&, const HitInfo&);

	Engine::Rect GetBoundingBox() const;
	glm::vec2 GetPosition() const;
	float GetWidth() const { return m_Bounds.x; }
	float GetHeight() const { return m_Bounds.y; }


	bool HasCollided{false};

private:
	bool doDebugRendering{ false };
	glm::vec2 m_Bounds{ 0,0 };
	glm::vec2 m_Velocity{ 0,0 };
};

