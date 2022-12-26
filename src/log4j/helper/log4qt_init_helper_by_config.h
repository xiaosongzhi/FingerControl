#ifndef LOG4QT_HELPER_BY_CONFIG_H
#define LOG4QT_HELPER_BY_CONFIG_H
#include <QString>
//启动log4qt库，使用配置文件的方式配置log4qt
extern void SetupLog4QtByConfigWithConfigFileAbsPath(QString config_file_abs_path);
extern void ShutDownLog4QtByConfig();//关闭log4qt库
#endif // LOG4QT_HELPER_H
