#include <chrono>
#include <csignal>
#include <signal.h>
#include <functional>
#include <sys/stat.h>
#include <thread>

#include "checksum_worker.h"
#include "crc32.h"
#include "event_queue.h"
#include "notifier.h"
#include "params_parser.h"
#include "syslogger.h"
#include "timeout_worker.h"

volatile sig_atomic_t do_shutdown = 0;
// per-thread ?
void termHandler(int signal)
{
    do_shutdown = signal;
}

std::function<void()> recalculateWrapper;
void recalculateHandler(int value)
{
    recalculateWrapper();
}

int daemonize() {
    // Fork off the parent process
    pid_t pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Change the file mode mask
    // Since the child process is a daemon, the umask needs to be set so files and logs can be written
    umask(0);

    // Create a new SID for the child process
    pid_t sid = setsid();
    if (sid < 0) {
        // Log failure and exit
        syslog(LOG_ERR, "Could not generate session ID for child process");
        exit(EXIT_FAILURE);
    }

    // Close standard streams
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}


int main(int argc, char *argv[]) {
    std::shared_ptr<SyslogWriter> logger(new SyslogWriter(),  [](SyslogWriter* writer)
        {
            RemoveLogger();
            delete writer;
        });
    SetLogger(logger);
    
    using namespace CommandLine;
    CommandLineArgsParser::Option opt1{CommandLineArgsParser::ValueType::STRING};
    CommandLineArgsParser::Option opt2{CommandLineArgsParser::ValueType::INT};
    CommandLineArgsParser::Option opt3{CommandLineArgsParser::ValueType::NOVAL};
    std::unordered_map<std::string, CommandLine::CommandLineArgsParser::Option> options{
        {"directory", opt1},
        {"timeout", opt2},
        // {"debug", opt3}
    };
    CommandLineArgsParser parser(options);

    std::string directory;
    uint32_t timeout;
    try
    {
        parser.Parse(argc, (const char**)argv);
        directory = parser.GetValue<std::string>("directory");
        timeout = parser.GetValue<uint32_t>("timeout");
    }
    catch(const std::exception& e)
    {
        logger->writeLog(e.what());
        return -1;
    }

    // Start the daemon
    daemonize();

    // Set signal handlers
    std::signal(SIGTERM, termHandler);
    std::signal(SIGUSR1, recalculateHandler);
    std::signal(SIGQUIT, SIG_IGN);
    std::signal(SIGINT, SIG_IGN);
    std::signal(SIGHUP, SIG_IGN);
    std::signal(SIGSTOP, SIG_IGN);
    std::signal(SIGCONT, SIG_IGN);

    const unsigned int numberOfThreads = 50;
    const unsigned int chunkSize = 8192;
    std::shared_ptr<Queue::EventQueue> queue = std::make_shared<Queue::EventQueue>(std::make_unique<Worker::ChecksumWorker>(numberOfThreads, chunkSize));
    INotifier::INotifyHandler notifyhandler(directory, queue);
    try
    {
        notifyhandler.Init();
    }
    catch(const std::exception& e)
    {
        logger->writeLog(e.what());
        return -1;
    }

    Timeout::TimeoutWorker timeoutWorker(timeout, directory, queue);
    recalculateWrapper = std::bind(std::mem_fn(&Timeout::TimeoutWorker::DoWork), &timeoutWorker, false);

    std::vector<std::thread> threads;
    threads.emplace_back(std::thread(&Queue::EventQueue::Start, queue));
    threads.emplace_back(std::thread(&INotifier::INotifyHandler::Start, &notifyhandler));
    threads.emplace_back(std::thread(&Timeout::TimeoutWorker::Start, &timeoutWorker));
 
    // just yield, main thread just for stopping others
    while (!do_shutdown)
        std::this_thread::yield();

    timeoutWorker.Stop();
    notifyhandler.Stop();
    queue->Stop();

    for (auto& t : threads)
        t.join();
    
    return 0;
}