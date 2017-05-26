#ifndef ANTLOG_H
#define ANTLOG_H

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/layout.h>
#include <log4cplus/consoleappender.h>
using namespace log4cplus;
using namespace log4cplus::helpers;

class AntLog
{
public:
    AntLog();
    void ldebug(std::string msg);
    void lerror(std::string msg);
    void linfo(std::string msg);
private:
    SharedObjectPtr<Appender> m_append;
    std::string m_pattern;
    std::auto_ptr<Layout> m_layout;
    Logger m_logger;
};

static AntLog logger;
/*
#define ldebug(msg) logger.ldebug(msg)
#define linfo(msg) logger.linfo(msg)
#define lerror(msg) logger.lerror(msg)
*/

#endif // ANTLOG_H
