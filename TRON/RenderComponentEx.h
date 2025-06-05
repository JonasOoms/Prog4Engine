#pragma once
#include "RenderComponent.h"

class RenderComponentEx : public RenderComponent
{
public:
	RenderComponentEx(float width, float height);
	virtual ~RenderComponentEx() = default;

	virtual void Render() override;

private:
	float m_Width, m_Height;
};

