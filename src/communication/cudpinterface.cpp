#include "cudpinterface.h"
#include <QUdpSocket>
#include "dataFormate.h"
#include "readconfig.h"
#include "logger.h"


//LOG4QT_DECLARE_STATIC_LOGGER(logger, CUdpInterface)
CUdpInterface::CUdpInterface():m_udpSocket(NULL)
{
    if(!m_udpSocket)
    {
        m_udpSocket = new QUdpSocket();
        connect(m_udpSocket,&QUdpSocket::readyRead,this,&CUdpInterface::receiveDataInterface);
        connect(m_udpSocket,&QUdpSocket::stateChanged,this,&CUdpInterface::deviceStateChanged);
        setConfigParam();
    }
}
CUdpInterface::~CUdpInterface()
{
    if(m_udpSocket)
    {
        delete m_udpSocket;
        m_udpSocket = NULL;
    }
}
//配置参数
bool CUdpInterface::setConfigParam()
{
    //读取配置文件
    int16_t port;
    QString ip;
    if(ReadConfig::getInstance()->getUdpServerAddress(port,ip))
    {
        if(!m_udpSocket->bind(QHostAddress(ip),port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint))
        {
//            logger()->debug(QString("UDP bind port %1 failed").arg(port));
        }
        else
        {
            qDebug()<<"bind successed"<<port<<ip;
        }
    }
    //绑定端口
    return true;
}
//发送数据接口
void CUdpInterface::sendDataInterface(QByteArray sendArray)
{
    int16_t port;
    QString ip;
    if(ReadConfig::getInstance()->getUdpClientAddress(port,ip))
    {
        if(m_udpSocket)
        {
            m_udpSocket->writeDatagram(sendArray,QHostAddress(ip),port);
        }
    }

}
//接收数据接口
void CUdpInterface::receiveDataInterface()
{
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray buf;
        buf.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(buf.data(),buf.size());
        if(buf[0] != (char)PACKHEAD)
        {
            //如果不是完成包则丢弃
            break;
        }
        else
        {
            //获取有效数据长度
            uint8_t datalen = 0;
            memcpy(&datalen,buf.constData()+1,sizeof(uint8_t));
            //当缓存中的数据长度大于等于一包数据长度时开始解析
            if(buf.length() >= datalen + 8)
            {
                uint8_t CRC_H = 0;
                uint8_t CRC_L = 0;
#if 0
                uint8_t *checkData = (uint8_t *)malloc(datalen+7);
                memset(checkData,0,datalen+7);
                memcpy(checkData,buf.constData(),datalen+7);
                Pressure_CheckCRC(checkData,datalen+7,&CRC_H,&CRC_L);
                free(checkData);
                checkData = NULL;
#else
                Pressure_CheckCRC((uint8_t*)buf.constData(),datalen+6,&CRC_H,&CRC_L);
#endif
                //校验成功
                if((CRC_L == (uint8_t)buf[6+datalen]) && (CRC_H == (uint8_t)buf[7+datalen]))
                {
                    emit  signalReadyRead(buf.mid(0,datalen + 8));
                    buf.clear();
                }
                else
                {
                    buf.clear();
                    break;
                }
            }
        }
    }
}

void CUdpInterface::deviceStateChanged(QAbstractSocket::SocketState state)
{
    switch(state)
    {
    case QAbstractSocket::BoundState:
        emit signalStateChanged(BoundState_E);
        break;
    case QAbstractSocket::ClosingState:
        emit signalStateChanged(ClosingState_E);
        break;
    default:
        break;
    }
}

//解析数据
void CUdpInterface::analysisProtocal(QByteArray)
{

}
