#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QPixmap>
#include "cdatabaseinterface.h"
#include "readconfig.h"
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok =ReadConfig::getInstance()->readConfigFile();
    if(!ok)
    {
        qDebug()<<"read config file failed";
        return -1;
    }

    //打开数据库
    if(!CDatabaseInterface::getInstance()->openDB("fingercontrol.db","QSQLITE"))
    {
        qDebug()<<"mysql open failed";
    }
    else
        qDebug()<<"musql open success";

    //加载样式文件
    QFile qssFile("./QSS/app.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        qDebug()<<"app.qss文件打开失败!";
    else
    {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }

    QPixmap startPix("./source/image/startpicture.png");
    QSplashScreen screen(startPix);

    screen.show();
    a.processEvents();

    MainWindow w;
    w.show();

    screen.finish(&w);
    w.switchUser();

    return a.exec();
}
