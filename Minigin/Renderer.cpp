#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <SDL_ttf.h>
#include "Font.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	// Setup ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
	

}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const SDL_Rect* srcRect) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), srcRect, &dst);

}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const SDL_Rect* srcRect, float angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), srcRect, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void dae::Renderer::DrawString(const std::string& text, Font* pFont, const glm::vec2& position, SDL_Color color, float size) const
{
	const auto surf = TTF_RenderText_Blended(pFont->GetFont(), text.c_str(), color);
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
	RenderTexture(*std::make_shared<Texture2D>(texture), position.x, position.y, size, size, nullptr, 0.f);
}

void dae::Renderer::DrawSquare(int x, int y, int size, SDL_Color color)
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), color.r, color.g, color.b, color.a);
	SDL_Rect square = { x, y, size, size };
	SDL_RenderDrawRect(GetSDLRenderer(), &square);
}

void dae::Renderer::DrawRect(Engine::Rect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), color.r, color.g, color.b, color.a);
	SDL_Rect square = { static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height) };
	SDL_RenderDrawRect(GetSDLRenderer(), &square);
}

void dae::Renderer::DrawPolygon(const std::vector<glm::vec2>& points, SDL_Color color)
{
		// Set the color
		SDL_SetRenderDrawColor(GetSDLRenderer(), color.r, color.g, color.b, color.a);

		std::vector<SDL_Point> sdlPoints;
		sdlPoints.reserve(points.size()); 

		for (const glm::vec2& glmPoint : points)
		{
			SDL_Point sdlPoint;
			sdlPoint.x = static_cast<int>(glmPoint.x);  
			sdlPoint.y = static_cast<int>(glmPoint.y);  
			sdlPoints.push_back(sdlPoint);
		}


		
		SDL_RenderDrawLines(GetSDLRenderer(), sdlPoints.data(),(int) sdlPoints.size());

		SDL_RenderDrawLine(GetSDLRenderer(), (int) points[sdlPoints.size() - 1].x, (int) points[sdlPoints.size() - 1].y, (int) points[0].x, (int) points[0].y);
}

void dae::Renderer::drawLine(int x1, int y1, int x2, int y2, SDL_Color color)
{
	SDL_Renderer* renderer = GetSDLRenderer();
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void dae::Renderer::DrawPoint(int x, int y, SDL_Color color) {
	SDL_Renderer* renderer = GetSDLRenderer();
	if (!renderer) return;

	// Set the drawing color
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	// Draw the point
	SDL_RenderDrawPoint(renderer, x, y);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
