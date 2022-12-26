#ifndef CLOG4J_H
#define CLOG4J_H

#include <QObject>
#include <log4qt/Logger>
#include <QMutex>

class CLog4j : public QObject
{
    Q_OBJECT
public:
    //获取实例
    static CLog4j* getInstance();
    //初始化日志配置
    bool init(QString configPath);

    void debug(QString msg);
    void info(QString msg);
    void warn(QString msg);
    void error(QString msg);
private:
    explicit CLog4j(QObject *parent = nullptr);

    static CLog4j *m_plog4j;
    static QMutex m_mutex;

    static Log4Qt::Logger *m_pLoggerDebug;
    static Log4Qt::Logger *m_pLoggerInfo;
    static Log4Qt::Logger *m_pLoggerWarn;
    static Log4Qt::Logger *m_pLoggerError;

    QString m_configPath;
};

#endif // CLOG4J_H
