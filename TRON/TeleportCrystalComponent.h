#pragma once
#include "PhysicsComponent.h"
#include "GridComponent.h"

class TeleportCrystalComponent : public PhysicsComponent
{
public:
	TeleportCrystalComponent(glm::vec2 bounds, Engine::Graph* graph);
	virtual void OnCollide(float dt, PhysicsComponent&, dae::GameObject&, const HitInfo&) override;
private:
	Engine::Graph* m_Graph;



};

