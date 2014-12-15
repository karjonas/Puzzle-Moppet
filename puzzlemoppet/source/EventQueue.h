
#include "IEventQueue.h"
#include <deque>

class EventQueue : public IEventQueue
{
	enum QUEUE_ITEM_TYPE
	{
		QIT_EVENT,
		QIT_TIME_WAIT,
		QIT_LOGIC_WAIT
	};
	
	struct QueueItem
	{
		QueueItem(QUEUE_ITEM_TYPE type)
			: type(type), neverFirstInQueue(true)
		{
		}
		
		QUEUE_ITEM_TYPE type;
		
		// used to detect when item becomes first in queue
		bool neverFirstInQueue;
		
		// Time a wait starts.
		// Set when an item becomes the first in the queue.
		f32 waitStartTime;
		
		// things specific to a particular type
		Event event;
		IWaitLogic *waitLogic;
		f32 waitTime;
	};
	
	u32 eventCount;
	std::deque<QueueItem> items;
	
public:
	EventQueue();
	~EventQueue();
	
	void AddEvent(const Event &event) override;
	
	void AddTimeWait(f32 waitTime) override;
	
	void AddLogicWait(IWaitLogic *waitLogic) override;
	
	u32 GetEventsWaiting() override;
	
	bool IsEmpty() override;
	
	bool IsEventWaiting(const core::stringc &eventName) override;
	
	void ScaleTimes(f32 scale) override;
	
	std::vector<Event *> GetAllEvents() override;
	
	void Update(f32 dt) override;
};

