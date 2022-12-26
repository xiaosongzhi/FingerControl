#include "log4qt_init_helper_by_config.h"
#include <QFile>
#include "log4qt.h"
#include "propertyconfigurator.h"
#include <log4qt/Logger>
#include <loggerrepository.h>
#include <QDebug>

void SetupLog4QtByConfigWithConfigFileAbsPath(QString config_file_abs_path)
{
    if (QFile::exists(config_file_abs_path)) {
        Log4Qt::PropertyConfigurator::configure(config_file_abs_path);
    } else {
        qDebug() << "Can't find log4qt-config-file path:" << config_file_abs_path;
    }
}
void ShutDownLog4QtByConfig()
{
    auto logger = Log4Qt::Logger::rootLogger();
    logger->removeAllAppenders();
    logger->loggerRepository()->shutdown();
}
