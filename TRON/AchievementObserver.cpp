#include "AchievementObserver.h"
#include "Event.h"

void AchievementObserver::Notify(const Event& event, EventDispatcher*)
{

	if (event.eventType == EventType::sdbm_hash("VALUE_CHANGED_EVENT"))
	{
		EventContext::ValueChangedEventContext<int> context = std::any_cast<EventContext::ValueChangedEventContext<int>>(event.eventContext);
		if (context.newValue >= 500)
		{
			
		}
	}
	
}
