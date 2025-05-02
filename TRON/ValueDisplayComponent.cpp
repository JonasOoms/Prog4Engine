#include "ValueDisplayComponent.h"
#include "Event.h"

ValueDisplayComponent::ValueDisplayComponent(const std::string& prefix, const std::string& suffix, int startValue) :
	TextRenderComponent{ prefix + std::to_string(startValue) + suffix, "Lingua.otf", 22 },
	m_Prefix{prefix},
	m_Suffix{suffix}
{
	
}

void ValueDisplayComponent::Notify(const Event& event, EventDispatcher* subject)
{
	subject;
	if (event.eventType == EngineEvents::EVENT_VALUE_CHANGED)
	{
		EventContext::ValueChangedEventContext<int> context = std::any_cast<EventContext::ValueChangedEventContext<int>>(event.eventContext);
		SetText(m_Prefix + std::to_string(context.newValue) + m_Suffix);
	}
}
