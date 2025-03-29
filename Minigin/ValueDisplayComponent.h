#pragma once
#include <string>
#include "TextRenderComponent.h"
#include "Observer.h"

class ValueDisplayComponent: public TextRenderComponent, public  Observer
{
public:
	ValueDisplayComponent(const std::string& prefix, const std::string& suffix, int startValue);

	// Inherited via Observer
	void Notify(const Event& event, EventDispatcher* subject) override;
private:
	std::string m_Prefix;
	std::string m_Suffix;
};

