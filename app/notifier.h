

#include "event_queue.h"

#pragma once

namespace INotifier
{
    class INotifyHandler : public Queue::Stoppable
    {
    public:
        explicit INotifyHandler(const std::string& folderPath, std::shared_ptr<Queue::EventQueue> queuePtr);
        ~INotifyHandler();
      
        void Init();
        void Start() override;
        void Stop() override;

        void handleFileEvent(const std::string& filename);
        
    private:
        int inotifyFd;
        int inotifyWd;
        std::atomic<bool> stop = false;
        std::shared_ptr<Queue::EventQueue> queue;
        const std::string dirPath;
    };

}