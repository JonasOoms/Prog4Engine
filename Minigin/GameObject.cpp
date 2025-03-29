
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

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

void dae::GameObject::Destroy()
{
	m_DeleteFlag = true;
}

bool dae::GameObject::GetDeleteFlag()
{
	return m_DeleteFlag;
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

dae::Transform dae::GameObject::GetTransform() const
{
	return m_transform;
}

