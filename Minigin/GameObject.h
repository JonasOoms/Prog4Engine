#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:

		void EndPlay();
		void BeginPlay();
		void Update(float deltaTime);
		void FixedUpdate(float fixedTime);
		void LateUpdate(float deltaTime);
		void DebugDraw() const;
		void Render() const;

		void Destroy();
		bool GetDeleteFlag();
		void SetPosition(float x, float y);
		glm::vec2 GetPosition();

		Transform& GetTransform();

		template<typename componentType>
		componentType* GetComponent();

		template<typename componentType>
		bool HasComponent();

		template<typename componentType, typename... Args>
		componentType* AddComponent(Args&&... args);

		template<typename componentType>
		bool RemoveComponent();

		bool IsParentOf(GameObject* object);
		void SetParent(GameObject* parent, bool keepWorldPosition, bool sendEvent = true);

  		GameObject* GetParent() { return m_Parent; }

		EventDispatcher* GetGameObjectEventDispatcher() const
		{
			return m_pGameObjectEventDispatcher.get();
		}

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		// If nullptr, is scene root
		GameObject* m_Parent{};
		std::vector<GameObject*> m_Children;

		bool m_DeleteFlag{};
		Transform m_transform;
		std::vector<std::unique_ptr<Component>> m_Components;
		std::unique_ptr<EventDispatcher> m_pGameObjectEventDispatcher;
		
		void RemoveChild(GameObject* object, bool sendEvent);
		void AddChild(GameObject* object, bool sendEvent);
	};

	// TODO: use concepts here
	template<typename componentType>
	inline componentType* GameObject::GetComponent()
	{
		auto itComponent = std::find_if(m_Components.begin(), m_Components.end(), [&](const std::unique_ptr<Component>& a) {
			return dynamic_cast<componentType*>(a.get()) != nullptr;
			});

		if (itComponent != m_Components.end())
		{
			return dynamic_cast<componentType*>(itComponent->get());
		}

		return nullptr;
	}

	template<typename componentType>
	inline bool GameObject::HasComponent()
	{
		auto itComponent = std::find_if(m_Components.begin(), m_Components.end(), [&](const std::unique_ptr<Component>& a) {
			auto aConverted = dynamic_cast<componentType*>(a.get());
			if (aConverted)
			{
				return true;
			}
			return false;
			});
		return (!(itComponent == m_Components.end()));
	}

	template<typename componentType, typename ...Args>
	inline componentType* GameObject::AddComponent(Args && ...args)
	{
		auto pComponent = std::make_unique<componentType>(std::forward<Args>(args)...);
		((Component*)pComponent.get())->SetOwner(this);
		m_Components.emplace_back(std::move(pComponent));
		return (componentType*)(m_Components.back().get());
	}

	template<typename componentType>
	inline bool GameObject::RemoveComponent()
	{
		auto itComponent = std::find_if(m_Components.begin(), m_Components.end(), [&](const std::unique_ptr<Component>& a) {
			auto aConverted = dynamic_cast<componentType*>(a.get());
			if (aConverted)
			{
				return true;
			}
			return false;
			});
		(*itComponent).get()->DeleteComponent();
		return true;
	}
}
