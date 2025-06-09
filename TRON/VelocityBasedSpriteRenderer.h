#pragma once
#include "RenderComponent.h"
#include "PhysicsComponent.h"

class VelocityBasedSpriteRenderer : public RenderComponent
{
public:
	VelocityBasedSpriteRenderer(float width, float height);
	virtual void Render();
private:
	float m_Width{};
	float m_Height{};
	PhysicsComponent* m_CachedPhysicsComponent{};
};

