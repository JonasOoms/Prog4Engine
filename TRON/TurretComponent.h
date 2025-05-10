#pragma once
#include "RenderComponent.h"
#include "Event.h"
#include "EventDispatcher.h"

class TurretComponent : public RenderComponent
{
public:
	TurretComponent(const std::string& turretTexturePath, float width, float height);
	void Render();
	void Update(float);
	void AddAngle(float angle);
	float GetAngle() const {return m_Angle; }
private:
	float m_Width{};
	float m_Height{};

	float m_Angle{0};

};


