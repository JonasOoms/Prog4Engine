#pragma once
#include "RenderComponent.h"
#include "Observer.h"
#include "GameEnums.h"

class TankRendererComponent : public RenderComponent, public Observer
{
public:

	TankRendererComponent(const std::string& tankTexturePath, float width, float height);


	virtual void ComponentOwnerInitialized() override;
	virtual void Render();
	virtual void Update(float deltaTime) override;
private:
	TRONEnums::Direction m_Direction{ TRONEnums::Direction::Left };
	float m_width{};
	float m_height{};


	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;

};

