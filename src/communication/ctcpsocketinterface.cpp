#include "ctcpsocketinterface.h"
#include <QTcpSocket>
#include <QApplication>
#include <QTcpServer>
#include <QAbstractSocket>
CTcpSocketInterface::CTcpSocketInterface(int type):
    m_tcpSocket(NULL),
    m_tcpServer(NULL)
{
    if(0 == type)   //客户端
    {
        m_tcpSocket = new QTcpSocket();
        connect(m_tcpSocket,&QTcpSocket::readyRead,this,&CTcpSocketInterface::receiveDataInterface);
    }
    else if(1 == type)  //服务端
    {
        m_tcpServer = new QTcpServer();
        connect(m_tcpServer, &QTcpServer::newConnection, this, &CTcpSocketInterface::newTcpConnection);
        connect(m_tcpServer, &QTcpServer::acceptError, this, &CTcpSocketInterface::displayError);
    }
}

//新的连接
void CTcpSocketInterface::newTcpConnection()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    if(m_tcpSocket)
    {
        connect(m_tcpServer, &QTcpServer::newConnection, this, &CTcpSocketInterface::newTcpConnection);
        connect(m_tcpServer, &QTcpServer::acceptError, this, &CTcpSocketInterface::displayError);
    }
}
//错误输出
void CTcpSocketInterface::displayError(QAbstractSocket::SocketError socketError)
{
    QString lastError("");
    switch(socketError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        lastError = "ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        lastError = "RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        lastError = "HostNotFoundError";
        break;
    case QAbstractSocket::SocketAccessError:
        lastError = "SocketAccessError";
        break;
    case QAbstractSocket::UnknownSocketError:
        lastError = "UnknownSocketError";
        break;
    default:
        break;
    }
    emit signalDisplayError(lastError);
}

//配置参数
bool CTcpSocketInterface::setConfigParam()
{
    //读取xml配置文件
    QString path = QApplication::applicationDirPath();
    //绑定端口
    return true;
}
//发送数据接口
void CTcpSocketInterface::sendDataInterface(QByteArray sendArray)
{
    if(m_tcpSocket)
    {
        m_tcpSocket->write(sendArray);
    }
}
//解析数据
void CTcpSocketInterface::analysisProtocal(QByteArray dataArray)
{
    Q_UNUSED(dataArray)
}

void CTcpSocketInterface::receiveDataInterface()
{
    QByteArray sendArray;
    emit signalReadyRead(sendArray);
}
