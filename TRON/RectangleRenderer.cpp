#include "RectangleRenderer.h"
#include "ResourceManager.h" 
#include "GameObject.h"
#include "Renderer.h"

RectangleRendererComponent::RectangleRendererComponent(float width, float height, const std::string& path):
	m_Width{width},
	m_Height{height}
{
	SetTexture(dae::ResourceManager::GetInstance().LoadTexture(path));
}

void RectangleRendererComponent::Render()
{
	glm::vec2 position = GetOwner()->GetPosition();
	dae::Renderer::GetInstance().RenderTexture(*GetTexture(), position.x, position.y, m_Width, m_Height, nullptr, 0);
}