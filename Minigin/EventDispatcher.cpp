#include "EventDispatcher.h"
#include <algorithm>
#include "Observer.h"
#include "Event.h"

void EventDispatcher::AddObserver(Observer* observer)
{
	if (!(std::find(m_Observers.begin(), m_Observers.end(), observer) != m_Observers.end()))
	{
		m_Observers.emplace_back(observer);
	}
}

void EventDispatcher::RemoveObserver(Observer* observer)
{
		std::erase(m_Observers, observer);
}

void EventDispatcher::NotifyObservers(const Event& event)
{
	if (!m_Observers.empty())
	{
		for (const auto& observer : m_Observers)
		{
			observer->Notify(event, this);
		}
	}
}
