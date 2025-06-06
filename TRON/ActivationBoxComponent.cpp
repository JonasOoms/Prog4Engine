#include "ActivationBoxComponent.h"
#include "PhysicsComponent.h"
#include "EngineUtils.h"

ActivationBoxComponent::ActivationBoxComponent(std::function<void()> action)
{
	m_Action = action;
}

void ActivationBoxComponent::Activate(dae::GameObject* activator)
{

	PhysicsComponent* activatorPhysicsComponent = activator->GetComponent<PhysicsComponent>();
	PhysicsComponent* activationBoxPhysicsComponent = GetOwner()->GetComponent<PhysicsComponent>();

	assert(activatorPhysicsComponent && activationBoxPhysicsComponent && "Activator and ActivationBox must both have a physics component.");

	if (Engine::CheckAABBCollision(activationBoxPhysicsComponent->GetBoundingBox(), activatorPhysicsComponent->GetBoundingBox()))
	{
		m_Activated = true;
	}

}

void ActivationBoxComponent::Update(float)
{
	if (m_Activated)
	{
		m_Action();
		m_Activated = false;
	}
}

ActivateCommand::ActivateCommand(dae::GameObject* activator, std::vector<ActivationBoxComponent*>&& activationBoxes):
	m_Activator{activator},
	m_ActivationBoxes{std::move(activationBoxes)}
{
}

void ActivateCommand::Execute()
{
	for (ActivationBoxComponent* activationBox : m_ActivationBoxes)
	{
		activationBox->Activate(m_Activator);
	}
}
