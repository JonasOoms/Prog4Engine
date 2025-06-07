#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject* owner)
{
	m_Owner = owner;
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
 	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

const glm::vec3& dae::Transform::GetLocalPosition()
{
	return m_LocalPosition;
}

void dae::Transform::SetLocalPosition(const glm::vec3& localPosition)
{
	m_LocalPosition = localPosition;
	m_PositionIsDirty = true;
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	m_PositionIsDirty = true;
}

void dae::Transform::SetRotation(float angle)
{
	m_Rotation = angle;
}

void dae::Transform::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void dae::Transform::SetIsDirty()
{
	m_PositionIsDirty = true;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_Owner->GetParent() == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = m_Owner->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition;
		}
			
	}
	m_PositionIsDirty = false;
}
