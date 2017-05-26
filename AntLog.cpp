#include "AntLog.h"
#include <string>

AntLog::AntLog():
    m_append(new ConsoleAppender()),
    m_pattern("%d{%m/%d/%y %H:%M:%S}  - %m [%l]%n"),
    m_layout(new PatternLayout(m_pattern))
{
    m_append->setLayout(m_layout);

    m_logger = Logger::getInstance("root");
    m_logger.addAppender(m_append);
    m_logger.setLogLevel(ALL_LOG_LEVEL);

    LOG4CPLUS_DEBUG(m_logger, "init logger success");
}

void AntLog::ldebug(std::__cxx11::string msg)
{
    LOG4CPLUS_DEBUG(m_logger, msg);
}

void AntLog::lerror(std::__cxx11::string msg)
{
    LOG4CPLUS_ERROR(m_logger, msg);
}

void AntLog::linfo(std::__cxx11::string msg)
{
    LOG4CPLUS_INFO(m_logger, msg);
}

