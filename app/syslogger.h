#include <syslog.h>

#include <memory>
#include <string>

#pragma once

class SyslogWriter
{
public:
    SyslogWriter()
    {
        openlog("MyDaemon", LOG_PID, LOG_DAEMON);
    }

    ~SyslogWriter()
    {
        closelog();
    }

    void writeLog(std::string message)
    {
        int priority = LOG_INFO;
        syslog(priority, "%s", message.data());
    }
};

std::shared_ptr<SyslogWriter> GetLogger();
void SetLogger(std::shared_ptr<SyslogWriter> logger);
void RemoveLogger();

