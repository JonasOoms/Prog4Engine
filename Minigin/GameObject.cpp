
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Event.h"

dae::GameObject::GameObject():
	m_transform{this},
	m_pGameObjectEventDispatcher{ std::make_unique<EventDispatcher>() }
{}

dae::GameObject::~GameObject() = default;

void dae::GameObject::EndPlay()
{
	for (const auto& component : m_Components)
	{
		component->EndPlay();
	}
}

void dae::GameObject::BeginPlay()
{
	for (const auto& component : m_Components)
	{
		component->BeginPlay();
	}
}

void dae::GameObject::Update(float deltaTime) {
	for (const auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float fixedTime) 
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate(fixedTime);
	}
}

void dae::GameObject::LateUpdate(float deltaTime)
{
	for (const std::unique_ptr<Component>& component : m_Components)
	{
		component->LateUpdate(deltaTime);
	}

	std::erase_if(m_Components, [](const std::unique_ptr<Component>& component)
		{
			return component->IsFlaggedForDelete();
		});
}

void dae::GameObject::DebugDraw() const
{
#ifdef _DEBUG
	for (auto& component : m_Components)
	{
		component->DebugDraw();
	}
#endif 
}

void dae::GameObject::Render() const
{
	for (const std::unique_ptr<Component>& component : m_Components)
	{
		// not 100% sure about dynamic cast.
		// Pros: this approach treats RenderComponents as anything but components, which keeps the logic simple for the component system
		// Cons: performance heavy when there are many components.

		RenderComponent* toRender = dynamic_cast<RenderComponent*>(component.get()); 
		if (toRender)
		{
			toRender->Render();
		}
	}
}

bool dae::GameObject::IsParentOf(GameObject* object)
{
	for (GameObject* child : m_Children)
	{
		if (child == object or child->IsParentOf(object))
		{
			return true;
		}
	}
	return false;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition, bool sendEvent)
{
	bool isValid{ !this->IsParentOf(parent) || !(parent == this) || !(m_Parent == parent)};
	assert(isValid);
	if (isValid)
	{
		if (parent == nullptr)
			m_transform.SetLocalPosition(m_transform.GetWorldPosition());
		else
		{
			if (keepWorldPosition)
				m_transform.SetLocalPosition(m_transform.GetWorldPosition() - parent->GetTransform().GetWorldPosition());
		}
		if (m_Parent)
		{
			m_Parent->RemoveChild(this, sendEvent);
		}
		m_Parent = parent;
		if (m_Parent)
		{
			m_Parent->AddChild(this, sendEvent);
		}
	}
}

void dae::GameObject::RemoveChild(GameObject* object, bool sendEvent)
{
	if (sendEvent)
	{
		EventContext::GameObjectChildHierarchyChangedContext ctx{ this, object };
		Event removeChildEvent{ EngineEvents::EVENT_GAMEOBJECT_CHILDREMOVED, ctx };
		m_pGameObjectEventDispatcher->NotifyObservers(removeChildEvent);
	}
	std::erase(m_Children, object);
}

void dae::GameObject::AddChild(GameObject* object, bool sendEvent)
{
	m_Children.emplace_back(object);
	if (sendEvent)
	{
		EventContext::GameObjectChildHierarchyChangedContext ctx{ this, object };
		Event addChildEvent{ EngineEvents::EVENT_GAMEOBJECT_CHILDADDED, ctx };
		m_pGameObjectEventDispatcher->NotifyObservers(addChildEvent);
	}
}

void dae::GameObject::Destroy()
{
	m_DeleteFlag = true;
	for (dae::GameObject* child : m_Children)
	{
		child->m_DeleteFlag = true;
	}
}

bool dae::GameObject::GetDeleteFlag()
{
	return m_DeleteFlag;
}


void dae::GameObject::SetPosition(float x, float y)
{
	Transform oldTransform = GetTransform();
	m_transform.SetPosition(x, y, 0.0f);
	for (GameObject* child : m_Children)
	{
		child->GetTransform().SetIsDirty();
	}
	Event event{ EngineEvents::EVENT_GAMEOBJECT_TRANSFORMCHANGED, EventContext::GameObjectTransformChangedEventContext{this, oldTransform, GetTransform()}};
	m_pGameObjectEventDispatcher->NotifyObservers(event);
}

glm::vec2 dae::GameObject::GetPosition()
{
	glm::vec3 worldPos = m_transform.GetWorldPosition();
	return glm::vec2{ worldPos.x, worldPos.y };
}

dae::Transform& dae::GameObject::GetTransform()
{
	return m_transform;
}

