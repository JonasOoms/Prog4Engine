#include "TankRendererComponent.h"
#include "Renderer.h"
#include "GameEvents.h"
#include "GameObject.h"

TankRendererComponent::TankRendererComponent(const std::string& tankTexturePath, float width, float height):
	RenderComponent(),
	m_width{ width },
	m_height{height}
{
	SetTexture(tankTexturePath);
}

void TankRendererComponent::ComponentOwnerInitialized()
{
	//GetOwner()->GetGameObjectEventDispatcher()->AddObserver(this);
}

void TankRendererComponent::Render()
{
	dae::Texture2D* texture = GetTexture();

	glm::ivec2 texSize = texture->GetSize();
	float frameSize = texSize.x / 4.f;
	SDL_Rect srcRect{static_cast<int>(frameSize*static_cast<int>(m_Direction)), 0, static_cast<int>(frameSize), texSize.y};


	glm::vec2 pos = GetOwner()->GetTransform().GetWorldPosition();

	dae::Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y, m_width, m_height, &srcRect, 0.f);
}

void TankRendererComponent::Update(float)
{
}

void TankRendererComponent::Notify(const Event& event , EventDispatcher* )
{
	if (event.eventType == TRONEvents::EVENT_PLAYER_MOVE)
	{
		TRONEvents::EventContexts::PlayerMoveEventContext context = std::any_cast<TRONEvents::EventContexts::PlayerMoveEventContext>(event.eventContext);
		glm::vec2 movementDelta = context.movementDelta;
		if (movementDelta.x > 0)
		{
			m_Direction = TRONEnums::Direction::Right;
		}
		else if (movementDelta.x < 0)
		{
			m_Direction = TRONEnums::Direction::Left;
		}
		else if (movementDelta.y > 0)
		{
			m_Direction = TRONEnums::Direction::Down;
		}
		else if (movementDelta.y < 0)
		{
			m_Direction = TRONEnums::Direction::Up;
		}

	} 
	if (event.eventType == EngineEvents::EVENT_GAMEOBJECT_TRANSFORMCHANGED)
	{
		EventContext::GameObjectTransformChangedEventContext context = std::any_cast<EventContext::GameObjectTransformChangedEventContext>(event.eventContext);
		glm::vec2 movementDelta = context.newTransform.GetWorldPosition() - context.oldTransform.GetWorldPosition();
		if (glm::length(movementDelta) > 0.5f)
		{
			if (movementDelta.x > 0)
			{
				m_Direction = TRONEnums::Direction::Right;
			}
			else if (movementDelta.x < 0)
			{
				m_Direction = TRONEnums::Direction::Left;
			}
			else if (movementDelta.y > 0)
			{
				m_Direction = TRONEnums::Direction::Down;
			}
			else if (movementDelta.y < 0)
			{
				m_Direction = TRONEnums::Direction::Up;
			}
		}
	}
}

