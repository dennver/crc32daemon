#include <memory>
#include <map>

#include "event_queue.h"
#include "syslogger.h"

#pragma once

namespace Worker
{
    class ChecksumWorker : public Queue::EventHandler
    {
        struct FileIntegrity
        {
            // "path" : "ABS_PATH_TO_FILE_1", // use path as map key
            uint32_t etalon_crc32; //" : "0XA59FCB0F",
            uint32_t result_crc32; //" : "0XA59FCB0F",
            std::string status; //"status " : "OK"
        };
    public:
        ChecksumWorker();
        ~ChecksumWorker() {}

        void HandleEvent(Queue::Event e) override;
        void DumpEvents() override;

    private:
        std::shared_ptr<SyslogWriter> logger;
        std::shared_ptr<Queue::EventQueue> queue;
        std::map<std::string, FileIntegrity> fileMap;
    };
}
