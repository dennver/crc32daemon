#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

#pragma once

namespace Queue
{
    class Stoppable
    {
    public:
        virtual ~Stoppable() {}
        virtual void Stop() {}
        virtual void Start() {}
    private:
        // std::atomic_notify_all
    };
    enum EventType {CREATED, MODIFIED, DELETED, UNKNOWN};
    struct Event
    {
        int dataSize;
        char* data;
        std::string fileName;
        EventType type;
        bool firstEvent;
    };

    class EventHandler : public Stoppable
    {
    public:
        EventHandler() {};
        virtual ~EventHandler() {};

        virtual void HandleEvent(Event e) = 0;
        virtual void DumpEvents() = 0;
    };

    class EventQueue : public Stoppable
    {
    public:
        EventQueue(std::unique_ptr<EventHandler> e) : eventHandler(std::move(e)) {}
        void AddEvent(Event e);

        void Start() override;
        void Stop() override;

    private:
        std::mutex mutex;
        std::condition_variable cv;
        std::atomic<bool> stop = false;
        std::queue<Event> eventQueue;
        std::unique_ptr<EventHandler> eventHandler;
    };


}