#pragma once
#include "RenderComponent.h"
#include "Observer.h"

class BulletRenderComponent : public RenderComponent, public Observer
{

	BulletRenderComponent(const std::string& bulletTexture);

	virtual void Render() override;

	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;
};

