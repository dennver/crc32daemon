
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

    return 0;
}