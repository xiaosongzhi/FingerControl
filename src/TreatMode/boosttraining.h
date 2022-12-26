#ifndef BOOSTTRAINING_H
#define BOOSTTRAINING_H

/*************助力训练***************/
/***助力训练的起始位置为伸展位置
 *助力运动中 四指和拇指分开运动
 *
 * ******/
#include "dataFormate.h"
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include "userdataformate.h"
#define DEADLINE 2
namespace Ui {
class BoostTraining;
}

class BoostTraining : public QWidget
{
    Q_OBJECT

public:
    explicit BoostTraining(QWidget *parent = nullptr);
    ~BoostTraining();

    void showTempData(ST_RealTimeFingersPosition&);

public slots:
    void slotTurnOffRunning(int pageIndex);

    //用户切换
    void slotCurrentUserChanged();
protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_fourFinger_Btn_clicked();

    void on_oneFinger_Btn_clicked();

    void on_level1_Btn_clicked();

    void on_level2_Btn_clicked();

    void on_level3_Btn_clicked();

    void receiveCommData(QByteArray);

    void on_start_Btn_clicked();

    void on_finish_Btn_clicked();

    //检测一个小流程
    void slotTimerCheckProcess();

    //检测整个大流程是否训练完成
    void slotTimerTrainingCompleted();

    void on_times_RadioButton_clicked();

    void on_duration_RadioButton_clicked();

    //手指状态发生变化
    void slotFingerStateChanged(int8_t index,int8_t state);

    //倒计时条
    void slotTimeOutProgressbar();
    void on_stretch_ProgressBar_valueChanged(int value);

    void on_buckling_ProgressBar_valueChanged(int value);

private:
    void initWidget();

    /**************************
     * 设置训练手指(四指或者拇指)
     * 参数@int8_t fingerType  1-四指 0-拇指
     * 返回值void
     * ***************************/
    void setMoveFinger(int8_t fingerType);

    /**************************
     * 设置助力训练等级
     * 参数@int8_t level  1,2,3三个等级
     * 返回值void
     * ***************************/
    void setBoostTrainLevel(int8_t level);

    /**************************
     * 助力训练开关
     * 参数@bool isStart  true-开始训练 false-停止训练
     * 返回值void
     * ***************************/
    void switchBoostTrain(bool isStart);

    //设置手指移动位置
    void setHandFingersPosition(ST_HandFingersPosition st_FingerPosition);

    //告知手指移动到目标位置
    void moveToDestLimitPosition(ST_PassiveDestPosition st_passiveDestPosition);

    //开启流程
    void startTrainProcess();

    //显示运动方向 -1-屈曲  1-伸展
    void showMoveDirection(int8_t flag);

    //判断是否达到指定位置
    bool chargeIsArrivedDest(ST_RealTimeFingersPosition&);

    //计算屈曲和伸展的最大值
    void calculateStretchAndBluckling();

    //设置力量显示
    void setStrengthShow(int8_t type,uint8_t strengthPercent);

    //关节活动度曲线
    void showScopeChart();

    //设置测试控件的显隐
    void setTestVisible(bool isShow);
private:
    Ui::BoostTraining *ui;
    QString boostFourNormal;
    QString boostFourSelected;
    QString boostOneNormal;
    QString boostOneSelected;

    QStringList levelNormalList;
    QStringList levelSelectedList;

    QTimer *m_processTimer; //单流程过程检测函数
    QTimer *m_isTrainingCompletedTimer;//完整流程检查是否完成训练,刷新频率较高
    QTimer *m_progressBarTimer;

    int m_handType;         //左右手

    int m_fingerType;       //1-四指  0-拇指

    int m_destFunction;   //目标方向 1-上边界  -1下边界

    bool m_isArrivedDest; //记录上位机控制是否到达目标位置

    int m_destDuration;     //目标训练时长
    int m_trainTimes;       //目标训练次数

    int currentDuration;    //当前训练时长
    int currentTrainTimes;  //当前训练次数

    int8_t currentPositionFlag;//当前位置

    ST_DestPosition st_DestPosition;//目标位置

    int8_t countType;   //计数方式 1-按次数  2-按时间

    int totalTrainTime; //总训练时长

    int m_fourTotalScope;   //四指总活动范围
    int m_oneTotalScope;    //拇指总活动范围

    ST_HandFingersPosition st_handPosition;//手指位置数据

    int originalSteate[5];//手指的原始状态
    int fingersState[5];//手指当前状态 0-disEnable 1-Enable

    QList<uint16_t> stretchList;    //伸展数据记录链表
    QList<uint16_t> bucklingList;   //屈曲数据记录链表

    QDateTime startTime;
    QDateTime endTime;

    ST_TrainReport st_trainReport; //训练报告
    ST_PassiveDestPosition st_passiveDestPosition;

    bool isUsedBoost;    //是否使用了助力

    int currentTime;     //当前计时
    int totalTimeout;    //总时间

    bool m_isTraining;  //是否在训练中

};

#endif // BOOSTTRAINING_H
