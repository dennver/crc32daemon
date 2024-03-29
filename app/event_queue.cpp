#include <iostream>
#include <thread>


#include "event_queue.h"
#include "crc32.h"
#include "syslogger.h"

using namespace std;
namespace Queue
{
    void EventQueue::AddEvent(Event e)
    {
        std::lock_guard<std::mutex> lock(mutex);
        eventQueue.push(e);
        cv.notify_all();
    }

    void EventQueue::Start()
    {
        std::shared_ptr<SyslogWriter> logger = GetLogger();
        while (!stop.load(std::memory_order_relaxed))
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [=] { return !eventQueue.empty() || stop.load(std::memory_order_relaxed); });
            if (stop.load(std::memory_order_relaxed))
                return;
            Event event = eventQueue.front();
            eventQueue.pop();
            lock.unlock();

            eventHandler->HandleEvent(event);
        }
    }

    void EventQueue::Stop()
    {
        stop.store(true, std::memory_order_relaxed);
        cv.notify_all();
        eventHandler->DumpEvents();
    }

}


