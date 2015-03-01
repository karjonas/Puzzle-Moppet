
#include "EventQueue.h"

EventQueue::EventQueue()
{
	eventCount = 0;
}

EventQueue::~EventQueue()
{
	for (u32 i = 0; i < items.size(); i ++)
	{
		if (items[i].type == QIT_LOGIC_WAIT)
			items[i].waitLogic->drop();
	}
}

void EventQueue::AddEvent(const Event &event)
{
	QueueItem item(QIT_EVENT);
	item.event = event;
	
	items.push_back(item);
	eventCount ++;
}

void EventQueue::AddTimeWait(f32 waitTime)
{
	QueueItem item(QIT_TIME_WAIT);
	item.waitTime = waitTime;
	
	items.push_back(item);
}

void EventQueue::AddLogicWait(IWaitLogic *waitLogic)
{
	ASSERT(waitLogic);
	
	QueueItem item(QIT_LOGIC_WAIT);
	item.waitLogic = waitLogic;
	
	waitLogic->grab();
	
	items.push_back(item);
}

u32 EventQueue::GetEventsWaiting()
{
	// this is incremented and decremented when events are added and removed
	return eventCount;
}

bool EventQueue::IsEmpty()
{
	return items.size() == 0;
}

bool EventQueue::IsEventWaiting(const core::stringc &eventName)
{
	// Search queue
	
	for (auto & elem : items)
	{
		if (elem.type == QIT_EVENT && elem.event.IsType(eventName))
			return true;
	}
	
	return false;
}

void EventQueue::ScaleTimes(f32 scale)
{
	for (auto & elem : items)
	{
		if (elem.type == QIT_TIME_WAIT)
		{
			if (elem.neverFirstInQueue)
			{
				elem.waitTime *= scale;
			}
			else // if first in queue, scale time remaining only
			{
				f32 timePassed = GetVirtualTime() - elem.waitStartTime;
				f32 timeRemaining = elem.waitTime - timePassed;
				
				elem.waitTime = timePassed + timeRemaining * scale;
			}
		}
	}
}

std::vector<Event *> EventQueue::GetAllEvents()
{
	std::vector<Event *> eventPointers;
	
	for (auto & elem : items)
	{
		if (elem.type == QIT_EVENT)
			eventPointers.push_back(&elem.event);
	}
	
	return eventPointers;
}

void EventQueue::Update(f32 dt)
{
	IUpdatable::Update(dt);
	
	// We only process the first thing in the queue.
	
	// we use a loop here, so if any events etc are popped off, we immediately
	// check if the next one can also be popped off. (without waiting until the next update)
	while (items.size())
	{
		bool poppedOff = false;
		
		QueueItem &item = items.front();
		
		if (item.neverFirstInQueue)
		{
			item.waitStartTime = GetVirtualTime();
			item.neverFirstInQueue = false;
		}
		
		switch (item.type)
		{
		// Events are handled as soon as they reach the front of the queue.
		case QIT_EVENT:
			GetEngine()->QueueEvent(item.event);
			items.pop_front();
			poppedOff = true;
			eventCount --;
			break;
		// Time waits hold up the queue until their wait time has passed.
		case QIT_TIME_WAIT:
			if (GetVirtualTime() > item.waitStartTime + item.waitTime)
			{
				items.pop_front();
				poppedOff = true;
			}
			break;
		// Logic wait holds up until its callback returns true.
		case QIT_LOGIC_WAIT:
			if (item.waitLogic->IsWaitOver(this, item.waitStartTime))
			{
				item.waitLogic->drop();
				items.pop_front();
				poppedOff = true;
			}
			break;
		}
		
		if (!poppedOff)
			break;
	}
}

