#pragma once
#include <Component.h>
#include <functional>
#include <vector>
#include "Command.h"

class ActivationBoxComponent : public Component
{
public:

	ActivationBoxComponent(std::function<void()> action);
	virtual void Activate(dae::GameObject* activator);
	virtual void Update(float);

private:
	bool m_Activated{};
	std::function<void()> m_Action;
};

class ActivateCommand : public Command
{
public:
	ActivateCommand(dae::GameObject* activator, std::vector<ActivationBoxComponent*>&& activationBoxes);
	void Execute() override;
private:
	dae::GameObject* m_Activator;
	std::vector<ActivationBoxComponent*> m_ActivationBoxes;
};