#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm.hpp>
#include <vector>
#include "ResourceManager.h"
namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const SDL_Rect* srcRect = nullptr) const;
		void RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const SDL_Rect* srcRect = nullptr, float angle = 0.f) const;

		void DrawString(const std::string& text, Font* pFont, const glm::vec2& position, SDL_Color color, float size = 12.f) const;
		void DrawSquare(int x, int y, int size, SDL_Color color);
		void DrawPolygon(const std::vector<glm::vec2>& points, SDL_Color color);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

