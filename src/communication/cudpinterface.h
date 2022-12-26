#ifndef CUDPINTERFACE_H
#define CUDPINTERFACE_H

#include "ccommunicationinterface.h"
#include <QObject>
#include <QAbstractSocket>
class QUdpSocket;
class CUdpInterface : public CCommunicationInterface
{
public:
    CUdpInterface();
    ~CUdpInterface();
    //配置参数
    virtual bool setConfigParam();
    //发送数据接口
    virtual void sendDataInterface(QByteArray);
    //解析数据
    virtual void analysisProtocal(QByteArray);
public slots:
    //接收数据接口
    void receiveDataInterface();
    //设备状态发生变化
    void deviceStateChanged(QAbstractSocket::SocketState);
    //设备错误
    void displayError(QAbstractSocket::SocketError socketError);
private:
    QUdpSocket *m_udpSocket;
};

#endif // CUDPINTERFACE_H
