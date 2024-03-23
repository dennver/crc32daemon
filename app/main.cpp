#include <iostream>
#include <getopt.h>

#include <csignal>


// gracefull shutdown
void signal_handler(int signum)
{
   exit(signum);  
}

int main(int argc, char *argv[])
{
    // Install a signal handler
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
        {0, 0, 0, 0}                           };

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
        case '?':
            if (optopt == 'd')
                std::cout << "Option -" << optopt << " requires an argument." << std::endl;
            else if (isprint(optopt))
                std::cout << "Unknown option `-" << optopt << "'.\n" << std::endl;
            else
                std::cout << "Unknown option character `" << optopt << "'." << std::endl;
            return 1;
        default:
            std::cout << "Incorrect prompt parameters";
            return 1;
        }
    }
    return 0;
}