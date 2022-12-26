#include "currentuserdata.h"
#include "cdatabaseinterface.h"
#include <QDebug>

CurrentUserData* CurrentUserData::m_currentUserData = NULL;


CurrentUserData::CurrentUserData(QObject *parent)
    : QObject(parent)

{
    QPair<int,int> pair;
    for(int i = 0;i < 5;i++)
    m_fingersRangeMap.insert(i,pair);
}

CurrentUserData* CurrentUserData::getInstace()
{
    if(!m_currentUserData)
    {
        m_currentUserData = new CurrentUserData();
    }
    return m_currentUserData;
}

//设置当前用户ID,在选择用户的时候调用
void CurrentUserData::setCurrentUserID(uint32_t ID)
{
    m_CurrentUserID = ID;

    qDebug()<<"更新显示用户";
    emit signalUserChanged();
}

//获取患者信息
ST_PatientMsg CurrentUserData::getCurrentPatientMsg()
{
    updateCurrentPatientMsg();
    return st_CurrentUserData;
}

//获取当前患者当前时间的训练报告
ST_TrainReport CurrentUserData::getCurrentTrainReport()
{
    return st_CurrentTrainReport;
}

void CurrentUserData::updateCurrentPatientMsg()
{
    //从数据库中查询数据
    QString queryStr= QString("select * from patientmessagetable where ID = '%1'").arg(m_CurrentUserID);

    if(CDatabaseInterface::getInstance()->exec(queryStr))
    {
        if(CDatabaseInterface::getInstance()->getValuesSize() > 0)
        {
            QList<QVariantMap> valueMapList;
            valueMapList = CDatabaseInterface::getInstance()->getValues(0,1);
            st_CurrentUserData = variantMapToPatientMsg(valueMapList.at(0));
            //更新训练用户
            qDebug()<<"更新训练用户";
        }
        else
        {
            qDebug()<<"未查询到符合条件的数据";
        }
    }
    else
    {
        qDebug()<<"updateCurrentPatientMsg failed"<<CDatabaseInterface::getInstance()->getLastError();
    }
}

void CurrentUserData::updateTrainUser()
{
    emit signalTrainUserChanged();
}

void CurrentUserData::setCurrentTrainReport(const ST_TrainReport&st_TrainReport)
{
    st_CurrentTrainReport = st_TrainReport;
}

//设置当前用户手数据
void CurrentUserData::setCurrentUserHandData(ST_HandData st_HandData)
{
    st_CurrentUserHandData = st_HandData;
    if(st_HandData.fingers.contains(0))
        st_CurrentUserHandData.fingerSelected[0] = 1;
    else
        st_CurrentUserHandData.fingerSelected[0] = 0;
    if(st_HandData.fingers.contains(1))
        st_CurrentUserHandData.fingerSelected[1] = 1;
    else
        st_CurrentUserHandData.fingerSelected[1] = 0;
    if(st_HandData.fingers.contains(2))
        st_CurrentUserHandData.fingerSelected[2] = 1;
    else
        st_CurrentUserHandData.fingerSelected[2] = 0;
    if(st_HandData.fingers.contains(3))
        st_CurrentUserHandData.fingerSelected[3] = 1;
    else
        st_CurrentUserHandData.fingerSelected[3] = 0;
    if(st_HandData.fingers.contains(4))
        st_CurrentUserHandData.fingerSelected[4] = 1;
    else
        st_CurrentUserHandData.fingerSelected[4] = 0;

}
//获取当前用户手数据
ST_HandData CurrentUserData::getCurrentUserHandData()
{
    return st_CurrentUserHandData;
}

//设置临时用户基本信息
void CurrentUserData::setTempUserMsg(ST_PatientMsg st_tempPatientMsg)
{
    st_TempCurrentUserMsg = st_tempPatientMsg;
    setCurrentUserID(st_TempCurrentUserMsg.ID);
}

void CurrentUserData::setCurrentUserMsg(ST_PatientMsg st_patientMsg)
{
    setCurrentUserID(st_patientMsg.ID);
}

ST_PatientMsg CurrentUserData::getTempUserMsg()
{
    return st_TempCurrentUserMsg;
}
//设置临时用户手数据
void CurrentUserData::setTempUserHandData(ST_HandData st_TempHandData)
{
    for(int i = 0;i < 5;i++)
    {
        if(st_TempHandData.fingers.contains(0))
            st_TempHandData.fingerSelected[i] = 1;
        else
            st_TempHandData.fingerSelected[i] = 0;
    }

    st_TempCurrentUserHandData = st_TempHandData;
}
ST_HandData CurrentUserData::getTempUserHandData()
{
    return st_TempCurrentUserHandData;
}

uint32_t CurrentUserData::getCurrentUserID()
{
    return m_CurrentUserID;
}

void CurrentUserData::setFingersRangeMap(QMap<int,QPair<int,int>> &map)
{
    m_fingersRangeMap = map;
}
void CurrentUserData::setFingerOppositeRange(QMap<int,QPair<int,int>> map)
{
    m_fingersOppositeRangeMap = map;
}
//获取上限值
ST_DestPosition CurrentUserData::getMaxDestPosition()
{
    ST_DestPosition st_DestPositon;
    if(1 == st_CurrentUserHandData.hand)
        st_DestPositon.thumb = m_fingersRangeMap.value(0).first;
    else if(0 == st_CurrentUserHandData.hand)
        st_DestPositon.thumb = m_fingersRangeMap.value(0).second;
    st_DestPositon.array[0] = st_DestPositon.thumb;
    st_DestPositon.forefinger = m_fingersRangeMap.value(1).second;
    st_DestPositon.array[1] = st_DestPositon.forefinger;
    st_DestPositon.middlefinger = m_fingersRangeMap.value(2).second;
    st_DestPositon.array[2] = st_DestPositon.middlefinger;
    st_DestPositon.ringfinger = m_fingersRangeMap.value(3).second;
    st_DestPositon.array[3] = st_DestPositon.ringfinger;
    st_DestPositon.littlefinger = m_fingersRangeMap.value(4).second;
    st_DestPositon.array[4] = st_DestPositon.littlefinger;

    return st_DestPositon;
}
//获取下限值
ST_DestPosition CurrentUserData::getMinDestPosition()
{
    ST_DestPosition st_DestPositon;
    if(1 == st_CurrentUserHandData.hand)
        st_DestPositon.thumb = m_fingersRangeMap.value(0).second;
    else if(0 == st_CurrentUserHandData.hand)
        st_DestPositon.thumb = m_fingersRangeMap.value(0).first;
    st_DestPositon.array[0] = st_DestPositon.thumb;
    st_DestPositon.forefinger = m_fingersRangeMap.value(1).first;
    st_DestPositon.array[1] = st_DestPositon.forefinger;
    st_DestPositon.middlefinger = m_fingersRangeMap.value(2).first;
    st_DestPositon.array[2] = st_DestPositon.middlefinger;
    st_DestPositon.ringfinger = m_fingersRangeMap.value(3).first;
    st_DestPositon.array[3] = st_DestPositon.ringfinger;
    st_DestPositon.littlefinger = m_fingersRangeMap.value(4).first;
    st_DestPositon.array[4] = st_DestPositon.littlefinger;

    return st_DestPositon;
}

QMap<int,QPair<int,int>> CurrentUserData::getFingersRangeMap()
{
    return m_fingersRangeMap;
}

QMap<int,QPair<int,int>> CurrentUserData::getOppositeRange()
{
    return m_fingersOppositeRangeMap;
}
