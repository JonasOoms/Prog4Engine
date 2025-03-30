#pragma once
#include "Component.h"
#include "TextRenderComponent.h"

class FPSComponent final: public Component
{
public:

	FPSComponent();
	FPSComponent(TextRenderComponent* link);
	virtual void Update(float deltaTime) override;

	void SetLink(TextRenderComponent* link);

private:
	TextRenderComponent* m_pLink;
};

