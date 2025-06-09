#include "VelocityBasedSpriteRenderer.h"
#include "Renderer.h"

VelocityBasedSpriteRenderer::VelocityBasedSpriteRenderer(float width, float height):
	RenderComponent(),
	m_Width{width},
	m_Height{height}
{
}

void VelocityBasedSpriteRenderer::Render()
{
	if (!m_CachedPhysicsComponent)
	{
		m_CachedPhysicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
	} 

	if (m_CachedPhysicsComponent)
	{
		glm::vec2 position = GetOwner()->GetPosition();
		glm::vec2 velocity = m_CachedPhysicsComponent->GetVelocity();
		float angle = glm::atan(velocity.y, velocity.x);
		dae::Renderer::GetInstance().RenderTexture(*GetTexture(), position.x, position.y, m_Width, m_Height, nullptr, angle);
	}

}
