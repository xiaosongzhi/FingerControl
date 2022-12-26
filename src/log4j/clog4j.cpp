 #include "clog4j.h"
#include "basicconfigurator.h"
#include "log4qt.h"
#include "log4qt/propertyconfigurator.h"

CLog4j *CLog4j::m_plog4j = NULL;
QMutex CLog4j::m_mutex;
Log4Qt::Logger *CLog4j::m_pLoggerDebug = NULL;
Log4Qt::Logger *CLog4j::m_pLoggerInfo = NULL;
Log4Qt::Logger *CLog4j::m_pLoggerWarn = NULL;
Log4Qt::Logger *CLog4j::m_pLoggerError = NULL;

CLog4j::CLog4j(QObject *parent) : QObject(parent)
{
    //调用基础配置
    Log4Qt::BasicConfigurator::configure();
}

CLog4j* CLog4j::getInstance()
{
    QMutexLocker mutexLocker(&m_mutex);
    if(!m_plog4j)
    {
        m_plog4j = new CLog4j();
    }
    return m_plog4j;
}

bool CLog4j::init(QString configPath)
{
    bool ok = Log4Qt::PropertyConfigurator::configure(configPath);
    if(ok)
    {
        m_pLoggerDebug = Log4Qt::Logger::logger("debug");
        m_pLoggerInfo = Log4Qt::Logger::logger("info");
        m_pLoggerWarn = Log4Qt::Logger::logger("warn");
        m_pLoggerError = Log4Qt::Logger::logger("error");
    }
    return false;
}

void CLog4j::debug(QString msg)
{
    m_pLoggerDebug->debug(msg);
}
void CLog4j::info(QString msg)
{
    m_pLoggerInfo->info(msg);
}
void CLog4j::warn(QString msg)
{
    m_pLoggerWarn->warn(msg);
}
void CLog4j::error(QString msg)
{
    m_pLoggerError->error(msg);
}
