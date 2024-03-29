#include "syslogger.h"

std::shared_ptr<SyslogWriter> l;
std::shared_ptr<SyslogWriter> GetLogger()
{
    return l;
}

void SetLogger(std::shared_ptr<SyslogWriter> logger)
{
   l = logger;
}

void RemoveLogger()
{
    l.reset();
}