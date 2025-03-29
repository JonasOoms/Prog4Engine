#include "TextRenderComponent.h"
#include <SDL_ttf.h>
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

TextRenderComponent::TextRenderComponent(const std::string& text, const std::string& fontPath, int size):
	m_Text{ text }
{
	m_pFont = dae::ResourceManager::GetInstance().LoadFont(fontPath, (uint8_t) size).get();
	m_ShouldUpdate = true;
}

TextRenderComponent::~TextRenderComponent()
{
}

void TextRenderComponent::Update(float deltaTime)
{
	deltaTime;
	if (m_ShouldUpdate)
	{

		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			//throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			//throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		SetTexture(std::move(std::make_shared<dae::Texture2D>(texture)));
		m_ShouldUpdate = false;
	}
}

void TextRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_ShouldUpdate = true;
}

void TextRenderComponent::SetFont(const std::string& fontPath, int size)
{
	m_pFont = dae::ResourceManager::GetInstance().LoadFont(fontPath, (uint8_t)size).get();
}

