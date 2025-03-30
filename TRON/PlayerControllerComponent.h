#pragma once
#include "Component.h"
#include <glm.hpp>

class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent(float speed);
	virtual ~PlayerControllerComponent() = default;
	virtual void Update(float elapsedSec) override;

	void AddMovementInput(const glm::vec2& movementInput);

private:
	bool m_IsDirty{};
	glm::vec2 m_MovementInput{};
	float m_Speed;
};

