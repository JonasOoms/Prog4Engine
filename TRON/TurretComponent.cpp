#include "TurretComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include <SDL.h>

TurretComponent::TurretComponent(const std::string& turretTexturePath, float width, float height)
{
}

void TurretComponent::Render()
{
	dae::Texture2D* texture = GetTexture();
	glm::vec2 pos = GetOwner()->GetTransform().GetPosition();
	dae::Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y, m_Width, m_Height, nullptr, m_Angle);
}

void TurretComponent::Update(float)
{
}
