#include "FPSComponent.h"

FPSComponent::FPSComponent()
{
	m_pLink = nullptr;
}

FPSComponent::FPSComponent(TextRenderComponent* link)
{
	m_pLink = link;
}

void FPSComponent::Update(float deltaTime)
{
	if (m_pLink)
	{
		m_pLink->SetText(std::to_string((int)round(1 / deltaTime)) + " FPS");
	}
}

void FPSComponent::SetLink(TextRenderComponent* link)
{
	m_pLink = link;
}
