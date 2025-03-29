#pragma once
#include <vector>

struct Event;
class Observer;

class EventDispatcher final
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
	virtual void NotifyObservers(const Event& event);
private:
	std::vector<Observer*> m_Observers;
};

