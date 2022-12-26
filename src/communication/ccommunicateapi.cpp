#include "ccommunicateapi.h"
#include "readconfig.h"
#include "ccommunicationinterface.h"
#include "cserialportinterface.h"
#include "cudpinterface.h"
#include "ctcpsocketinterface.h"
#include "dataFormate.h"
#include <QDebug>

CCommunicateAPI *CCommunicateAPI::m_API = NULL;

CCommunicateAPI::CCommunicateAPI(QObject *parent) :
    QObject(parent),
    m_interface(NULL)
{
    init();
}
CCommunicateAPI::~CCommunicateAPI()
{
    if(m_interface)
    {
        delete m_interface;
        m_interface = NULL;
    }
}

CCommunicateAPI *CCommunicateAPI::getInstance()
{
    if(!m_API)
    {
        m_API = new CCommunicateAPI();
    }
    return m_API;
}

void CCommunicateAPI::init()
{
    commuType = ReadConfig::getInstance()->getCommunicateType();
    //根据通信方式生成不同的对象
    if(0 == commuType) //串口
    {
        m_interface = new CSerialportInterface();
    }
    else if(1 == commuType) //udp
    {
        m_interface = new CUdpInterface();
    }
    else if(2 == commuType) //tcp
    {
        m_interface = new CTcpSocketInterface();
    }
    else if(3 == commuType) //can
    {
        //待添加
    }
    if(m_interface)
    {
        connect(m_interface,SIGNAL(signalReadyRead(QByteArray)),this,SIGNAL(signalReadyRead(QByteArray)));
    }
}

void CCommunicateAPI::sendData(QByteArray sendArray)
{
    sendArray.prepend(PACKHEAD);
    //添加校验
    uint8_t CRC_H = 0;
    uint8_t CRC_L = 0;
    Pressure_CheckCRC((uint8_t*)(sendArray.data()),sendArray.length(),&CRC_H,&CRC_L);
    sendArray.append(CRC_L);
    sendArray.append(CRC_H);
    m_interface->sendDataInterface(sendArray);

    emit signalSendData(sendArray);
}

void CCommunicateAPI::sendData(uint8_t seq,uint8_t id,uint16_t cmdID,QByteArray arrayData)
{
    QByteArray fixedArray;
    fixedArray.resize(5);
    fixedArray[0] = arrayData.length();
    fixedArray[1] = seq;
    fixedArray[2] = id;
    memcpy(fixedArray.data()+3,&cmdID,sizeof(uint16_t));

    fixedArray.append(arrayData);
    sendData(fixedArray);
}
//故障复位
void CCommunicateAPI::resetFault()
{
    QByteArray sendArray;
    sendArray.resize(4);
    sendArray[0] = 0;
    sendArray[1] = 0;
    sendArray[2] = 1;
    sendArray[3] = 0x06;

    sendData(sendArray);
}
//训练模式设定
void CCommunicateAPI::setTrainMode(E_TRAINMODE e_mode)
{
    uint16_t cmdId = SETTRAINMODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(1);
    array[0] = e_mode;
    sendData(seq,id,cmdId,array);
}

void CCommunicateAPI::setFingersPositon(ST_FingersPosition st_FingersPosition)
{
    qDebug()<<st_FingersPosition.thumb<<st_FingersPosition.forefinger<<st_FingersPosition.middlefinger<<st_FingersPosition.ringfinger<<st_FingersPosition.ringfinger;
    QByteArray arraData;
    arraData.resize(sizeof(ST_FingersPosition));
    memcpy(arraData.data(),&st_FingersPosition,arraData.length());
    sendData(0,1,TESTFINGERSPOSITION,arraData);
}

//控制手指上下移动
void CCommunicateAPI::setFingerMove(ST_SetFingerPosiotion st_SetFingerPosition)
{
    QByteArray arraData;
    arraData.resize(sizeof(ST_SetFingerPosiotion));
    memcpy(arraData.data(),&st_SetFingerPosition,arraData.length());
    sendData(0,1,CONTROLSIGLEFINGER,arraData);
}

//设置手指锁定
void CCommunicateAPI::setFingerLock(bool isLock)
{
    int8_t lock;
    if(isLock)
        lock = 1;
    else
        lock = 0;
    QByteArray arrayData;
    arrayData.resize(1);
    arrayData[0] = lock;
    sendData(0,1,LOCKFINGERS,arrayData);
}

//获取手指的实时位置
void CCommunicateAPI::getRealTimePositon(bool isTurnOn)
{
    uint16_t cmdId = GETFINGERPOSITION;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(1);
    if(isTurnOn)
    {
        array[0] = 1;
        qDebug()<<"实时位置获取开";
    }
    else
    {
        array[0] = 0;
        qDebug()<<"实时位置获取关";
    }

    sendData(seq,id,cmdId,array);
}
//获取各手指力量大小
void CCommunicateAPI::getRealTimeStrength(bool isTurnOn)
{
    uint16_t cmdId = GETFINGERFORCE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(1);
    if(isTurnOn)
    {
        array[0] = 1;
        qDebug()<<"实时力量获取开";
    }
    else
    {
        array[0] = 0;
        qDebug()<<"实时力量获取关";
    }
    sendData(seq,id,cmdId,array);
}

//发送心跳
void CCommunicateAPI::sendHeartBeat()
{
    uint16_t cmdId = HEARTBEAT;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    sendData(seq,id,cmdId,array);
}
