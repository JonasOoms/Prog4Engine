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
	dae::Texture2D* GetTexture() const { return m_pTexture.get(); }
private:
	std::shared_ptr<dae::Texture2D> m_pTexture;
};

