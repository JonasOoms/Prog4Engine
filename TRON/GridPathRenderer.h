#pragma once
#include "RenderComponent.h"

class GridComponent;

class GridPathRenderComponent : public RenderComponent
{
public:
	GridPathRenderComponent(GridComponent* gridComponent);
	virtual void Render() override;
private:
	GridComponent* m_pGrid;

};

