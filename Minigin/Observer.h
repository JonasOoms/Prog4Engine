#pragma once

struct Event;
class EventDispatcher;


class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify(const Event& event, EventDispatcher* subject) = 0;
};

