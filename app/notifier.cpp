#include "notifier.h"

#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>

namespace INotifier
{
    INotifyHandler::INotifyHandler(const std::string& folderPath, std::shared_ptr<Queue::EventQueue> queuePtr) : queue(queuePtr), dirPath(folderPath) {}

    INotifyHandler::~INotifyHandler()
    {
        if (inotifyWd >= 0)
            inotify_rm_watch(inotifyFd, inotifyWd);

        if (inotifyFd >= 0)
            close(inotifyFd);
    }

    void INotifyHandler::Init()
    {
        // Structure which would store the metadata
        struct stat sb;
    
        // Calls the function with path as argument
        // If the file/directory exists at the path returns 0
        // If block executes if path exists
        if (stat(dirPath.data(), &sb) != 0)
            throw std::runtime_error("no such directory: " + dirPath);

        inotifyFd = inotify_init();
        if (inotifyFd < 0) {
            throw std::runtime_error("Failed to initialize inotify");
        }

        inotifyWd = inotify_add_watch(inotifyFd, dirPath.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
        if (inotifyWd < 0) {
            throw std::runtime_error("Failed to add watch for folder: " + dirPath);
        }
    }

    void INotifyHandler::Start()
    {
        constexpr size_t EVENT_BUF_LEN = 1024;
        char buffer[EVENT_BUF_LEN];

        while (!stop.load(std::memory_order_relaxed))
        {
            fd_set input_set;
            struct timeval timeout;

            FD_ZERO(&input_set);
            FD_SET(inotifyFd, &input_set);

            timeout.tv_sec = 3;  // Timeout after 3 seconds
            timeout.tv_usec = 0;
            int ready = select(inotifyFd+1, &input_set, NULL, NULL, &timeout);

            if (ready > 0 && FD_ISSET(inotifyFd, &input_set))
            {
                ssize_t bytesRead = read(inotifyFd, buffer, EVENT_BUF_LEN);

                if (bytesRead < 0)
                {
                    // std::cerr << "Error reading inotify events" << std::endl;
                    break;
                }

                size_t i = 0;
                while (i < static_cast<size_t>(bytesRead))
                {
                    struct inotify_event* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);
                    
                    if (event->mask & (IN_MODIFY | IN_CREATE | IN_DELETE)) {
                        Queue::Event e;
                        e.fileName = dirPath + event->name;
                        queue->AddEvent(e);
                    }

                    i += sizeof(struct inotify_event) + event->len;
                }
            }
        }
    }

    void INotifyHandler::Stop()
    {
        stop.store(true, std::memory_order_relaxed);
    }
}