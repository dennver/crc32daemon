#include <chrono>
#include <csignal>
#include <signal.h>
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
// gracefull shutdown 
void signal_handler(int status)
{
    do_shutdown = status;
}

void signalHandler(int signal) {
    switch(signal) {
        case SIGHUP:
            // Handle SIGHUP signal
            break;
        case SIGTERM:
            // Handle SIGTERM signal
            exit(0);
            break;
    }
}

int daemonize() {
    // Fork off the parent process
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

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
        {"--directory", opt1},
        {"--timeout", opt2},
        // {"--debug", opt3}
    };
    CommandLineArgsParser parser(options);

    try
    {
        parser.Parse(argc, (const char**)argv);
    }
    catch(const std::exception& e)
    {
        logger->writeLog(e.what());
        return -1;
    }

    std::string directory = parser.GetValue<std::string>("--directory");
    int timeout = parser.GetValue<uint32_t>("--timeout");

    // Start the daemon
    daemonize();

    // Set signal handlers
    if (std::signal(SIGTERM, signal_handler) == SIG_ERR)  
       return -1;
    std::signal(SIGQUIT, SIG_IGN);
    std::signal(SIGINT, SIG_IGN);
    std::signal(SIGHUP, SIG_IGN);
    std::signal(SIGSTOP, SIG_IGN);
    std::signal(SIGCONT, SIG_IGN);
    int opt   = 0;
    int index = 0;
    std::string dir;

    static struct option long_options[] =
    {
        {"directory", optional_argument, 0, 'd'},
        {"version",   no_argument,       0, 'v'},
        {0, 0, 0, 0}                           
    };

    while ((opt = getopt_long(argc, argv, "d:v", long_options, &index)) != -1) {
        switch(opt) {
        // -d, --directory <directory name> dir path
        case 'd':
        {
            std::cout << optarg << std::endl;
            dir = optarg;
        }
            break;
        case 'v':
            std::cout << "Version: " << std::endl;
            return 0;
        case 0: // long option without a short arg
            break;
        default:
            std::cout << "Incorrect prompt parameters";
            return 1;
        }
    }
    
    return 0;
}