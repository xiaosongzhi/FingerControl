#ifndef CURRENTUSERDATA_H
#define CURRENTUSERDATA_H

#include <QObject>
#include "userdataformate.h"


class CurrentUserData : public QObject
{
    Q_OBJECT
public:
    static CurrentUserData* getInstace();

    //设置当前用户ID,在选择用户的时候调用
    void setCurrentUserID(uint32_t ID);
    uint32_t getCurrentUserID();

    //获取患者信息
    ST_PatientMsg getCurrentPatientMsg();
    void setCurrentUserMsg(ST_PatientMsg);

    //获取当前患者当前时间的训练报告
    ST_TrainReport getCurrentTrainReport();
    void setCurrentTrainReport(const ST_TrainReport&);

    //设置当前用户手数据
    void setCurrentUserHandData(ST_HandData);
    //获取当前用户手数据
    ST_HandData getCurrentUserHandData();

    /************临时用户是指还没有存入数据库的用户**************/
    //设置临时用户基本信息
    void setTempUserMsg(ST_PatientMsg);
    ST_PatientMsg getTempUserMsg();
    //设置临时用户手数据
    void setTempUserHandData(ST_HandData);
    ST_HandData getTempUserHandData();

    //设置当前用户的左右手 0-左手  1-右手
    void setFingersRangeMap(QMap<int,QPair<int,int>> &map);
    void setFingerOppositeRange(QMap<int,QPair<int,int>> map);
    //获取当前用户手指活动范围
    QMap<int,QPair<int,int>> getFingersRangeMap();
    QMap<int,QPair<int,int>>getOppositeRange();
    //获取上限值
    ST_DestPosition getMaxDestPosition();
    //获取下限值
    ST_DestPosition getMinDestPosition();

    //更新训练用户
    void updateTrainUser();
signals:
    //当前用户显示发生变化
    void signalUserChanged();
    //训练用户发生变化
    void signalTrainUserChanged();

private:
    void updateCurrentPatientMsg();

private:
    explicit CurrentUserData(QObject *parent = nullptr);

    static CurrentUserData* m_currentUserData;
    //当前用户ID
    uint32_t m_CurrentUserID;
    //当前用户certificateNum
    QString certificateNum;
    //当前用户手数据
    ST_HandData st_CurrentUserHandData;
    //当前用户信息
    ST_PatientMsg st_CurrentUserData;
    //当前用户当前时间的训练报告
    ST_TrainReport st_CurrentTrainReport;

    //存数据库之前的临时数据
    ST_PatientMsg st_TempCurrentUserMsg;
    ST_HandData st_TempCurrentUserHandData;

    QMap<int,QPair<int,int>> m_fingersRangeMap;
    QMap<int,QPair<int,int>> m_fingersOppositeRangeMap;

};

#endif // CURRENTUSERDATA_H
