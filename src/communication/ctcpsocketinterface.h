#ifndef CTCPSOCKETINTERFACE_H
#define CTCPSOCKETINTERFACE_H

#include "ccommunicationinterface.h"
#include <QObject>
#include <QAbstractSocket>
class QTcpSocket;
class QTcpServer;

class CTcpSocketInterface : public CCommunicationInterface
{
public:
    //0-客户端  1-服务端
    CTcpSocketInterface(int type = 0);

    //配置参数
    virtual bool setConfigParam();
    //发送数据接口
    virtual void sendDataInterface(QByteArray);
    //解析数据
    virtual void analysisProtocal(QByteArray);

public slots:
    //接收数据接口
    void receiveDataInterface();
    void newTcpConnection();
    void displayError(QAbstractSocket::SocketError socketError);
    void deviceStateChanged(QAbstractSocket::SocketState);
private:
    QTcpSocket *m_tcpSocket;
    QTcpServer *m_tcpServer;
};

#endif // CTCPSOCKETINTERFACE_H
