#ifndef CSERIALPORTINTERFACE_H
#define CSERIALPORTINTERFACE_H

#include "ccommunicationinterface.h"
#include <QObject>
#include <QSerialPort>
class CSerialportInterface : public CCommunicationInterface
{
public:
    CSerialportInterface();
    ~CSerialportInterface();
    //配置参数
    virtual bool setConfigParam();
    //发送数据接口
    virtual void sendDataInterface(QByteArray);
    //解析数据
    virtual void analysisProtocal(QByteArray);
public slots:
    //接收数据接口
   void receiveDataInterface();
   //通信状态变化
   void deviceStateChanged(int);
   //QSerialPort::SerialPortError
   //设备错误
   void displayError(QSerialPort::SerialPortError error);
private:
    QSerialPort *m_serialPort;
    QByteArray receiveArray;
};

#endif // CSERIALPORTINTERFACE_H
