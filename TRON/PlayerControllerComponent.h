#pragma once
#include "Component.h"
#include "EventDispatcher.h"
#include <glm.hpp>
#include "PhysicsComponent.h"

class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent(float speed);
	virtual ~PlayerControllerComponent() = default;
	virtual void ComponentOwnerInitialized() override;
	virtual void Update(float elapsedSec) override;


	void SetMovementInput(const glm::vec2& movementInput);
	void AddMovementInput(const glm::vec2& movementInput);

	EventDispatcher* GetMovedEvent() const { return m_MovedEvent.get(); }

private:
	
	void TriggerMovementEvents(const glm::vec2& movementDelta, const glm::vec2& newPosition);

	PhysicsComponent* m_CachedPhysicsComponent{};
	bool m_IsDirty{};
	glm::vec2 m_MovementInput{};
	float m_Speed;

	std::unique_ptr<EventDispatcher> m_MovedEvent;

};

