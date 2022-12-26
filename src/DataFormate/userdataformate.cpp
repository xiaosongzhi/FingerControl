#include "userdataformate.h"
#include <QVariantMap>
#include <QMapIterator>
#include <QUuid>
#include <QDebug>
extern ST_PatientMsg variantMapToPatientMsg(QVariantMap vMap)
{
    ST_PatientMsg st_PatientMsg;
    if(vMap.contains("ID"))
        st_PatientMsg.ID = vMap.value("ID").toUInt();
    if(vMap.contains("name"))
        st_PatientMsg.name = vMap.value("name").toString();
    if(vMap.contains("age"))
        st_PatientMsg.age = vMap.value("age").toUInt();
    if(vMap.contains("sex"))
        st_PatientMsg.sex = vMap.value("sex").toUInt();
    if(vMap.contains("height"))
        st_PatientMsg.height = vMap.value("height").toUInt();
    if(vMap.contains("weight"))
        st_PatientMsg.weight = vMap.value("weight").toUInt();
    if(vMap.contains("phone"))
        st_PatientMsg.phone = vMap.value("phone").toString();
    if(vMap.contains("hands"))
        st_PatientMsg.hands = vMap.value("hands").toInt();
    if(vMap.contains("hospitalDepartment"))
        st_PatientMsg.hospitalDepartment = vMap.value("hospitalDepartment").toString();
    if(vMap.contains("hospitalNumber"))
        st_PatientMsg.hospitalNumber = vMap.value("hospitalNumber").toString();
    if(vMap.contains("healthInsuranceType"))
        st_PatientMsg.healthInsuranceType = vMap.value("healthInsuranceType").toInt();
    if(vMap.contains("healthInsuranceNumber"))
        st_PatientMsg.healthInsuranceNumber = vMap.value("healthInsuranceNumber").toString();
    if(vMap.contains("address"))
        st_PatientMsg.address = vMap.value("address").toString();
    if(vMap.contains("certificateType"))
        st_PatientMsg.certificateType = vMap.value("certificateType").toInt();
    if(vMap.contains("certificateNum"))
        st_PatientMsg.certificateNum = vMap.value("certificateNum").toString();
    if(vMap.contains("remarkMessage"))
        st_PatientMsg.remarkMessage = vMap.value("remarkMessage").toString();
    return st_PatientMsg;
}

extern ST_JointRange variantMapToJointRange(QVariantMap vMap)
{
    ST_JointRange st_JointRange;
    if(vMap.contains("ID"))
        st_JointRange.ID = vMap.value("ID").toInt();
    if(vMap.contains("hand"))
        st_JointRange.hand = vMap.value("hand").toInt();
    if(vMap.contains("fingers"))
        st_JointRange.fingers = vMap.value("fingers").toString();

    for(int i = 0;i < 5;i++)
    {
        QString fingerIndex = QString("finger%1").arg(i);
        QString value = vMap.value(fingerIndex).toString();
        if(vMap.contains(fingerIndex))
        {
            QPair<int,int> rangePair;
            QStringList list = value.split('-');
            if(list.size() >1)
            {
                rangePair.first = list.at(0).toInt();
                rangePair.second = list.at(1).toInt();
                st_JointRange.fingersRange.insert(i,rangePair);
            }
        }
    }

    return st_JointRange;
}

extern ST_TrainReport variantMapToTrainReport(QVariantMap vMap)
{
    ST_TrainReport st_TrainReport;
    if(vMap.contains("ID"))
        st_TrainReport.ID = vMap.value("ID").toInt();
    if(vMap.contains("name"))
        st_TrainReport.name = vMap.value("name").toString();
    if(vMap.contains("level"))
        st_TrainReport.level = vMap.value("level").toInt();
    if(vMap.contains("startTime"))
        st_TrainReport.startTime = vMap.value("startTime").toDateTime();
    if(vMap.contains("endTime"))
        st_TrainReport.endTime = vMap.value("endTime").toDateTime();
    if(vMap.contains("trainMode"))
        st_TrainReport.trainMode = (E_TRAINMODE)vMap.value("trainMode").toInt();
    if(vMap.contains("duration"))
        st_TrainReport.duration = vMap.value("duration").toInt();
    if(vMap.contains("markMsg"))
        st_TrainReport.markMsg = vMap.value("markMsg").toString();
    if(vMap.contains("resultImagePath"))
        st_TrainReport.resultImagePath = vMap.value("resultImagePath").toString();
    return st_TrainReport;
}

extern ST_FringerStrengthResult variantMapToFringerStrengthResult(QVariantMap vMap)
{
    ST_FringerStrengthResult st_FringerStrength;
    if(vMap.contains("ID"))
        st_FringerStrength.ID = vMap.value("ID").toInt();
    if(vMap.contains("hand"))
        st_FringerStrength.hand = vMap.value("hand").toInt();
    if(vMap.contains("dateTime"))
        st_FringerStrength.dateTime = vMap.value("dateTime").toDateTime();
    if(vMap.contains("grip"))
        st_FringerStrength.grip = vMap.value("grip").toFloat();
    if(vMap.contains("tension"))
        st_FringerStrength.tension = vMap.value("tension").toFloat();
    for(int i = 0;i < 5;i++)
    {
        QString tempKey = QString("finger%1Strength").arg(i);
        if(vMap.contains(tempKey))
        {
            QString length = vMap.value(tempKey).toString();
            QStringList strengthList = length.split(',');
            if(!strengthList.isEmpty() && strengthList.at(0).toFloat() != -1)
            {
                QPair<float,float> lengthPair;
                lengthPair.first = strengthList.at(0).toFloat();
                lengthPair.second = strengthList.at(1).toFloat();
                st_FringerStrength.mapFingerStrength.insert(i,lengthPair);
            }

        }
    }
    if(vMap.contains("resultImagePath"))
        st_FringerStrength.resultImagePath = vMap.value("resultImagePath").toString();
    return st_FringerStrength;
}
extern ST_JointRangeResult variantMapToJointRangeResult(QVariantMap vMap)
{
    ST_JointRangeResult st_JointRangeResult;

    if(vMap.contains("ID"))
        st_JointRangeResult.ID = vMap.value("ID").toInt();
    if(vMap.contains("hand"))
        st_JointRangeResult.hand = vMap.value("hand").toInt();
    if(vMap.contains("startTime"))
        st_JointRangeResult.startDateTime = vMap.value("startTime").toDateTime();
    if(vMap.contains("endTime"))
        st_JointRangeResult.endDateTime = vMap.value("endTime").toDateTime();
    if(vMap.contains("totalPercent"))
        st_JointRangeResult.totalPercent = vMap.value("totalPercent").toInt();
    for(int i = 0;i < 5;i++)
    {
        QString tempRange = QString("finger%1Percent").arg(i);
        if(vMap.contains(tempRange))
        {
            int tempPercent = vMap.value(tempRange).toInt();
            if(-1 != tempPercent)
            {
                st_JointRangeResult.mapMovePercent.insert(i,tempPercent);
            }
        }
    }
    if(vMap.contains("resultImagePath"))
        st_JointRangeResult.resultImagePath = vMap.value("resultImagePath").toString();
    return st_JointRangeResult;
}

extern QVariantMap patientMsgToVariantMap(ST_PatientMsg st_PatientMsg)
{
    QVariantMap vMap;
    vMap.insert("ID",st_PatientMsg.ID);
    vMap.insert("name",st_PatientMsg.name);
    vMap.insert("age",st_PatientMsg.age);
    vMap.insert("sex",st_PatientMsg.sex);
    vMap.insert("height",st_PatientMsg.height);
    vMap.insert("weight",st_PatientMsg.weight);
    vMap.insert("hands",st_PatientMsg.hands);
    vMap.insert("hospitalDepartment",st_PatientMsg.hospitalDepartment);
    vMap.insert("hospitalNumber",st_PatientMsg.hospitalNumber);
    vMap.insert("healthInsuranceType",st_PatientMsg.healthInsuranceType);
    vMap.insert("healthInsuranceNumber",st_PatientMsg.healthInsuranceNumber);
    vMap.insert("phone",st_PatientMsg.phone);
    vMap.insert("address",st_PatientMsg.address);
    vMap.insert("remarkMessage",st_PatientMsg.remarkMessage);
    vMap.insert("certificateType",st_PatientMsg.certificateType);
    vMap.insert("certificateNum",st_PatientMsg.certificateNum);
    return vMap;
}

extern QVariantMap jointRangeToVariantMap(ST_JointRange st_JointRange)
{
    QVariantMap vMap;
    //添加UUID
    QUuid id = QUuid::createUuid();
    QString strId = id.toString(QUuid::Id128);
    vMap.insert("UUID",strId);
    vMap.insert("ID",st_JointRange.ID);
    vMap.insert("hand",st_JointRange.hand);
    vMap.insert("fingers",st_JointRange.fingers);

    QMapIterator<int,QPair<int,int>> iter(st_JointRange.fingersRange);
    while(iter.hasNext())
    {
        iter.next();
        QString value = QString::number(iter.value().first) + "-" + QString::number(iter.value().second);
        vMap.insert(QString("finger%1").arg(iter.key()),value);
    }
    return vMap;
}

extern QVariantMap trainReportToVariantMap(ST_TrainReport st_TrainReport)
{
    QVariantMap vMap;
    //添加UUID
    QUuid id = QUuid::createUuid();
    QString strId = id.toString(QUuid::Id128);
    vMap.insert("UUID",strId);
    vMap.insert("ID",st_TrainReport.ID);
    vMap.insert("name",st_TrainReport.name);
    vMap.insert("level",st_TrainReport.level);
    vMap.insert("startTime",st_TrainReport.startTime);
    vMap.insert("endTime",st_TrainReport.endTime);
    vMap.insert("trainMode",st_TrainReport.trainMode);
    vMap.insert("duration",st_TrainReport.duration);
    vMap.insert("markMsg",st_TrainReport.markMsg);
    vMap.insert("resultImagePath",st_TrainReport.resultImagePath);
    return vMap;
}

extern QVariantMap fringerStrengthResultToVariantMap(ST_FringerStrengthResult st_FringerStrength)
{
    QVariantMap vMap;
    //添加UUID
    QUuid id = QUuid::createUuid();
    QString strId = id.toString(QUuid::Id128);
    vMap.insert("UUID",strId);
    vMap.insert("ID",st_FringerStrength.ID);
    vMap.insert("hand",st_FringerStrength.hand);
    vMap.insert("dateTime",st_FringerStrength.dateTime);
    vMap.insert("grip",st_FringerStrength.grip);
    vMap.insert("tension",st_FringerStrength.tension);
    vMap.insert("resultImagePath",st_FringerStrength.resultImagePath);
    QMapIterator<int,QPair<float,float>> iter(st_FringerStrength.mapFingerStrength);
    while(iter.hasNext())
    {
        iter.next();
        QString tempKey = QString("finger%1Strength").arg(iter.key());
        QString tempValue = QString::number(iter.value().first) + "," + QString::number(iter.value().second);
        vMap.insert(tempKey,tempValue);
    }

    return vMap;
}

//手指关节度评定结果
extern QVariantMap jointRangeResultToVariantMap(ST_JointRangeResult st_JointRangeResult)
{
    QVariantMap vMap;
    //添加UUID
    QUuid id = QUuid::createUuid();
    QString strId = id.toString(QUuid::Id128);
    vMap.insert("UUID",strId);
    vMap.insert("ID",st_JointRangeResult.ID);
    vMap.insert("hand",st_JointRangeResult.hand);
    vMap.insert("startTime",st_JointRangeResult.startDateTime);
    vMap.insert("endTime",st_JointRangeResult.endDateTime);
    vMap.insert("totalPercent",st_JointRangeResult.totalPercent);
    vMap.insert("resultImagePath",st_JointRangeResult.resultImagePath);
    QMapIterator<int,int> iter(st_JointRangeResult.mapMovePercent);

    while(iter.hasNext())
    {
        iter.next();
        QString tempKey = QString("finger%1Percent").arg(iter.key());
        vMap.insert(tempKey,iter.value());
    }
    return vMap;
}
