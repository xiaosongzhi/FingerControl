#ifndef CCOMMUNICATEAPI_H
#define CCOMMUNICATEAPI_H

#include <QObject>
#include "dataFormate.h"
class CCommunicationInterface;
class CCommunicateAPI : public QObject
{
    Q_OBJECT
public:
    static CCommunicateAPI *getInstance();

    void sendData(QByteArray);

    /************************************
    *说明:发送数据函数【重载函数】
    *参数：
    *@uint8_t seq：包的序号,默认为0，当发送的数据需要拆包时才会使用
    *@uint8_t id ：自身设备ID号
    *@uint8_t cmdID：协议ID号
    *@QByteArray arrayData：有效数据
    *返回值：无
    ***********************************/
    void sendData(uint8_t seq,uint8_t id,uint16_t cmdID,QByteArray arrayData);

    //故障复位
    void resetFault();
    //训练模式设定
    void setTrainMode(E_TRAINMODE);
    //设置五指的位置
    void setFingersPositon(ST_FingersPosition);
    //控制手指上下移动
    void setFingerMove(ST_SetFingerPosiotion);
    //设置手指锁定
    void setFingerLock(bool isLock);
    //获取手指的实时位置
    void getRealTimePositon(bool isTurnOn);
    //获取各手指力量大小
    void getRealTimeStrength(bool isTurnOn);
    //心跳开关
    void sendHeartBeat();

signals:
    //接收到实际数据
    void signalReadyRead(QByteArray);

    void signalSendData(QByteArray);
private:

    explicit CCommunicateAPI(QObject *parent = nullptr);
    ~CCommunicateAPI();
    void init();

private:
    static CCommunicateAPI *m_API;
    int commuType;
    CCommunicationInterface *m_interface;
};

#endif // CCOMMUNICATEAPI_H
