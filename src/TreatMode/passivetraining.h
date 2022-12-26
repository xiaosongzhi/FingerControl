#ifndef PASSIVETRAINING_H
#define PASSIVETRAINING_H

#include <QWidget>
#include "dataFormate.h"
#include <QTimer>
#include <QDateTime>
#include "userdataformate.h"

/***********被动训练说明************
*手指的起始位置为伸展位置,对于下位机来说伸展位置都为小值
*
*力量暂定为0,6,12,18,24,30五个档位
*
* 伸展-正值  屈曲-负值
**/

namespace Ui {
class PassiveTraining;
}

class PassiveTraining : public QWidget
{
    Q_OBJECT

public:
    explicit PassiveTraining(QWidget *parent = nullptr);
    ~PassiveTraining();

public slots:
    void slotTurnOffRunning(int pageIndex);

    //用户切换
    void slotCurrentUserChanged();
protected:
    virtual void showEvent(QShowEvent *event);
private slots:
    void receiveCommData(QByteArray);

    void on_allFingers_Btn_clicked();

    void on_fourToOne_Btn_clicked();

    void on_leftToRight_Btn_clicked();

    void on_rightToLeft_Btn_clicked();

    void on_switchBucklingFace_Btn_clicked();

    void on_switchStretchFace_Btn_clicked();

    //时间间隔
    void on_interval_Dial_valueChanged(int value);
    //时间延时
    void on_delay_SpinBox_valueChanged(int arg1);

    void on_speed_Dial_valueChanged(int value);

    void on_rang_Dial_valueChanged(int value);

    void on_strength_Dial_valueChanged(int value);

    void on_start_Btn_clicked();

    void on_pause_Btn_clicked();

    void on_finish_Btn_clicked();

    //手指状态发生变化
    void slotFingerStateChanged(int8_t index,int8_t state);

    void on_verticalScrollBar_valueChanged(int value);

    //流程超时槽函数
    void slotProcessTimeOut();

    //流程是否完成
    void slotFinishTimer();
private:
    void initWidget();

    /******************
     *设置被动训练手指
     *参数@ST_HandTypeFingers   st_HandTypeFingers 需要训练的手指
     *返回值:void
     * ****************/
    void setPassiveTrainFinger(const ST_HandTypeFingers &st_HandTypeFingers);

    /******************
     *设置被动训练参数
     *参数@ST_PassiveParam  st_PassiveParam 需要训练的手指
     *返回值:void
     * ****************/
    void setPassiveTrainParam(const ST_PassiveParam  &st_PassiveParam);

    /******************
     *控制被动训练开关
     *参数@bool isStart   true-开始训练  false-结束训练
     *返回值:void
     * ****************/
    void switchPassiveTrain(bool isStart);

    //绘制力量域图
    void paintAreaChart(ST_RealTimeStrength&);

    //屈曲力打分
    void setBucklingScore(int strenth);
    //伸展力打分
    void setStretchScore(int strenth);

    //设置单个手指运动的位置
    void setFingerMoveToDestPosition(int8_t fingerIndex,uint8_t position);

    //训练流程
    void trainProcess();

    //设置各手指的位置
    void setHandFingersPosition(ST_HandFingersPosition&);

    //上下位置百分比
    void updateFingerMoveScope(float upDownPercent);
    //计算屈曲力量极值
    void calculateBucklingStrength(const ST_RealTimeStrength&);
    //计算伸展力量极值
    void calculateStretchStrength(const ST_RealTimeStrength&);
    //控制五个手指同时运动
    void controlFiveFingersMove( ST_FingersPosition&);

    //简单流程
    /*********五指同时活动***********/
    //从拇指到小指流程
    void formThumbToLittle();
    //从小指到拇指流程
    void fromLittleToThumb();
    /*********四指拇指分开活动*******/
    //从小指到食指
    void fromLittleToForefinger();
    //从食指到小指
    void fromForefingerToLittle();
    //运动拇指
    void thumbMoveProcess(int8_t direction);

    /******************
     *根据速度和距离计算延时时间
     *参数int speed 速度档位  int length 长度
     *返回值:void
     * ****************/
    void calculateTimeBySpeed(int speed,int length);

    //设置测试控件的显隐
    void setTestVisible(bool isShow);
    //清理控件
    void clearWidget();
    //数据同步
    void syncdata(ST_DestPosition&);
private:
    Ui::PassiveTraining *ui;

    int m_handType; //左右手

    ST_PassiveParam st_passiveParam;//被动训练参数

    int8_t m_fingerDirection;   //-1-从小指到拇指  0-悬空 1-拇指到小指
    int8_t m_actionType;        //-1-五指同时运动  0-悬空 1-先四指再拇指
    int8_t m_moveDirection;     //运动方向 -1向下屈曲位置  1-向上伸展位置
    //手指间的延迟时间
    int m_fingerInterval;
    //四指和拇指间的延迟
    int m_processDelay;
    //抓握总次数
    int m_totalTimes;
    //训练总时长
    int m_totalDuration;
    //当前抓握次数
    int m_currentTimes;
    //当前训练时长
    int m_currentDuration;
    //当前运动方向
    int8_t m_currentDirection;

    //活动范围边界值
    ST_DestPosition st_bucklingPosition;
    ST_DestPosition st_stretchPosition;
    //手指的原始状态
    int originalSteate[5];  //0-disEnable 1-Enable
    //手指当前状态
    int fingersState[5];    //0-disEnable 1-Enable

    //手指是否到达边界
    int8_t isArriveDestPosition[5];

    QTimer *m_positionTimer;    //位置定时器判断
    QTimer *m_timeOutTimer;     //超时定时器
    QTimer *m_finishTimer;      //根据当前设定的抓握次数和时间监控是否完成训练

    int  timeOutTime;           //超时时间（超时时间要根据当前设定的速度进行计算）
    //手指位置数据
    ST_HandFingersPosition st_handPosition;

    //要训练的活动度占总活动度的百分比
    float m_scopePercent; //0~1

    //手指的实际活动范围
    QMap<int,QPair<int,int>> map_realMoveScope;
    //基础范围
    QMap<int,QPair<int,int>> map_baseMoveScope;

    //屈曲力
    int m_bucklingStrength[5];
    int m_totalBucklingStrength;
    //伸展力
    int m_stretchStrength[5];
    int m_totalStretchStrength;

    QDateTime startTime;
    QDateTime endTime;

    ST_TrainReport st_trainReport; //训练报告

    //流程中是否到达目标位置
    bool m_isArrivedDestPosition;

    //最后一个手指的目标位置
    int m_lastFingerPosition;

    bool m_isTraining; //是否在训练中

    int8_t m_trainProcessType;//训练流程 -1-拇指流程  1-四指流程

//    int m_currentSpeed; //当前速度
    int m_lastSpeed;    //上次速度
};

#endif // PASSIVETRAINING_H
