#pragma once
#include <glm.hpp>
#include "EventDispatcher.h"

namespace dae
{

	class GameObject;

	class Transform final
	{
	public:

		Transform(GameObject* owner);

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition();
		void SetLocalPosition(const glm::vec3& localPosition);

		void SetPosition(float x, float y, float z);
		void SetRotation(float angle);
		void SetScale(float x, float y, float z);
		void SetIsDirty();
	private:

		void UpdateWorldPosition();

		bool m_PositionIsDirty{ false };

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		float m_Rotation{};
		glm::vec3 m_Scale{};

		dae::GameObject* m_Owner;
	};
}
