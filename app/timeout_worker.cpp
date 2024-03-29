#include "timeout_worker.h"

#include <chrono>
#include <thread>

namespace Timeout
{
    void TimeoutWorker::Start()
    {
        bool firstEvent = true;
        while (!stop.load(std::memory_order_relaxed))
        {

            const auto& files = traverser.Traverse();
            for (const auto& file : files)
            {
                Queue::Event e;
                e.fileName = file;
                e.firstEvent = firstEvent;
                queue->AddEvent(e);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            firstEvent = false;
        }
    }

    void TimeoutWorker::Stop()
    {
        stop.store(true, std::memory_order_relaxed);
    }

}