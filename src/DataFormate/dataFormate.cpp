#include "dataFormate.h"
#include <QTime>
#include <QCoreApplication>
void  Pressure_CheckCRC(uint8_t*buf,int len,uint8_t* CRC_H,uint8_t* CRC_L)
{
    uint16_t i,j,tmp,CRC16;
    CRC16=0xffff;
    for (i=0;i<len;i++)
    {
        CRC16=*buf^CRC16;
        for (j=0;j< 8;j++)
        {
            tmp=CRC16 & 0x0001;
            CRC16 =CRC16 >>1;
            if (tmp)
                CRC16=CRC16 ^ 0xA001;
        }
        *buf++;
    }
    CRC_H[0]=CRC16>>8;
    CRC_L[0]=CRC16&0xff;
}

void Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


