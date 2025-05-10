#pragma once
#include "RenderComponent.h"

class RectangleRendererComponent : public RenderComponent
{
public:
	RectangleRendererComponent(float width, float height, const std::string& path);

	virtual void Render() override;

private:
	float m_Width, m_Height;

};

