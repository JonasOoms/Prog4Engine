#pragma once
#include "Component.h"
#include "Texture2D.h"

class RenderComponent: public Component
{
public:

	RenderComponent();
	virtual void Render();

	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float fixedTime) override;

	void SetTexture(const std::string& texture);
protected:
	void SetTexture(std::shared_ptr<dae::Texture2D> texture);
private:
	std::shared_ptr<dae::Texture2D> m_pTexture;
};

