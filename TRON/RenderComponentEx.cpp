#include "RenderComponentEx.h"
#include "Renderer.h"
#include "GameObject.h"

RenderComponentEx::RenderComponentEx(float width, float height):
	m_Width{width},
	m_Height{height}
{
}

void RenderComponentEx::Render()
{
	dae::Renderer& renderer = dae::Renderer::GetInstance();
	glm::vec2 position = GetOwner()->GetPosition();
	renderer.RenderTexture(*GetTexture(), position.x, position.y, m_Width, m_Height);
}
