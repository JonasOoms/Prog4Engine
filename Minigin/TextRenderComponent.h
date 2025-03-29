#pragma once
#include "RenderComponent.h"
#include "Font.h"
#include "SDL.h"

class TextRenderComponent : public RenderComponent
{
public:

	TextRenderComponent(const std::string& text, const std::string& fontPath, int size);
	~TextRenderComponent();
	virtual void Update(float deltaTime) override;

	void SetText(const std::string& text);
	void SetFont(const std::string& fontPath, int size);



private:
	bool m_ShouldUpdate;
	std::string m_Text;
	dae::Font* m_pFont;
	SDL_Color m_Color{255,255,255,255};
	
	

};

