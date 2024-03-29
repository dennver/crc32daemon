#include "event_queue.h"
#include "dir_traverser.h"

#pragma once

namespace Timeout
{
    class TimeoutWorker : public Queue::Stoppable
    {
    public:
        TimeoutWorker(uint32_t t, const std::string& path, std::shared_ptr<Queue::EventQueue> queuePtr)
          : timeout(t), traverser(path), queue(queuePtr) {}
        void Start() override;
        void Stop() override;
        void DoWork(bool flag);
        
    private:
        uint32_t timeout;
        Traverser::DirectoryTraversal traverser;
        std::shared_ptr<Queue::EventQueue> queue;
        std::atomic<bool> stop = false;
    };
} // namespace name