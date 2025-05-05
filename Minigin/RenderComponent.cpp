#pragma once
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"


RenderComponent::RenderComponent()
{
}

void RenderComponent::Render()
{
	if (m_pTexture)
	{
		const auto& position = GetOwner()->GetTransform().GetWorldPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
	}
}

void RenderComponent::Update(float deltaTime) { deltaTime; }

void RenderComponent::FixedUpdate(float fixedTime) { fixedTime; }

void RenderComponent::SetTexture(const std::string& texture)
{
	m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texture);
}

void RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture)
{
	m_pTexture = std::move(texture);
}
