#ifndef CCOMMUNICATIONINTERFACE_H
#define CCOMMUNICATIONINTERFACE_H
#include <QObject>

class  CCommunicationInterface : public QObject
{
    Q_OBJECT
public:
    CCommunicationInterface();

    ~CCommunicationInterface();

    //配置参数
    virtual bool setConfigParam() = 0;
    //发送数据接口
    virtual void sendDataInterface(QByteArray) = 0;
    //解析数据
    virtual void analysisProtocal(QByteArray);
signals:
    //接收到实际数据
    void signalReadyRead(QByteArray);
    //通信对象发生错误
    void signalDisplayError(QString);
    //通信状态发生变化
    void signalStateChanged(int);
public slots:
    //接收数据接口

};

#endif // CCOMMUNICATIONINTERFACE_H
