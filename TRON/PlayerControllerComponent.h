#pragma once
#include "Component.h"
#include "EventDispatcher.h"
#include <glm.hpp>

class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent(float speed);
	virtual ~PlayerControllerComponent() = default;
	virtual void Update(float elapsedSec) override;

	void AddMovementInput(const glm::vec2& movementInput);

	EventDispatcher* GetMovedEvent() const { return m_MovedEvent.get(); }

private:
	
	void TriggerMovementEvents(const glm::vec2& movementDelta, const glm::vec2& newPosition);

	bool m_IsDirty{};
	glm::vec2 m_MovementInput{};
	float m_Speed;

	std::unique_ptr<EventDispatcher> m_MovedEvent;

};

