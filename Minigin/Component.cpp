#include "Component.h"
#include "GameObject.h"

Component::Component()
{
    m_pOwner = nullptr;
}

void Component::DeleteComponent()
{
    m_DeleteFlag = true;
}

void Component::SetOwner(dae::GameObject* owner)
{
	if (!m_pOwner)
	{
		m_pOwner = owner;
	}

	if (m_pOwner)
	{
		ComponentOwnerInitialized();
	}
}

dae::GameObject* Component::GetOwner() const
{
    return m_pOwner;
}
