#pragma once
#include "Component.h"
#include "EventDispatcher.h"
#include "Event.h"

template<typename T>
class ValueComponent : public Component
{
public:

	ValueComponent(T value = T{}):
	m_Value{value}
	{
		m_ValueChangedEvent = std::make_unique<EventDispatcher>();
	}

	T Get() const { return m_Value; }
	void Set(T value) { 
		m_Value = value; 
		SendEvent(m_Value);
	}
	void Increment(T value) {
		m_Value += value; 
		SendEvent(m_Value);
	}
	void Decrement(T value) {
		m_Value -= value; 
		SendEvent(m_Value);
	}

	void AddObserver(Observer* observer)
	{
		m_ValueChangedEvent->AddObserver(observer);
	}

	void RemoveObserver(Observer* observer)
	{
		m_ValueChangedEvent->RemoveObserver(observer);
	}

private:
	void SendEvent(T value);

	T m_Value;
	std::unique_ptr<EventDispatcher> m_ValueChangedEvent;

};

template<typename T>
inline void ValueComponent<T>::SendEvent(T value)
{
	Event valueChangedEvent;
	EventContext::ValueChangedEventContext<T> valueChangedContext{ value };
	valueChangedEvent.eventType = EventType::sdbm_hash("VALUE_CHANGED_EVENT");
	valueChangedEvent.eventContext = valueChangedContext;
	m_ValueChangedEvent->NotifyObservers(valueChangedEvent);
}
