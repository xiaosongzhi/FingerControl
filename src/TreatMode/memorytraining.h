#ifndef MEMORYTRAINING_H
#define MEMORYTRAINING_H

#include <QWidget>
#include "controlset.h"
#include "dataFormate.h"
#include <QLabel>
#include <QDateTime>
#include "userdataformate.h"
namespace Ui {
class MemoryTraining;
}

class MemoryTraining : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryTraining(QWidget *parent = nullptr);
    ~MemoryTraining();
private:
    void initWidget();

public slots:
    void slotTurnOffRunning(int pageIndex);

    //用户切换
    void slotCurrentUserChanged();
private slots:
    void receiveCommData(QByteArray);

    void on_bucklingType_Btn_clicked();

    void on_stretchType_Btn_clicked();

    void on_cycle_Btn_clicked();

    void on_level1_Btn_clicked();

    void on_level2_Btn_clicked();

    void on_level3_Btn_clicked();

    void on_visual_Btn_clicked();

    void on_controlSet_Btn_clicked();

    void on_start_Btn_clicked();

    void on_stop_Btn_clicked();
    
    void on_pushButton_clicked();

    //倒计时计时函数
    void slotTimerOut();
    //力量模式下判断运动是否正确
    void slotChargeForceDirection();
    //位置模式下判断是否正确
    void slotChargePosition();

    void on_moveTips_Btn_clicked();

    void on_goback_Btn_clicked();

    void on_moveTip_Btn_clicked();

    void on_test_Btn_clicked();

protected:
    void showEvent(QShowEvent *event);
private:
    /******************
     * 告知下位机开始/结束主动训练
     *参数@bool isStart   true-开始训练  false-结束训练
     *返回值:void
     * ****************/
    void switchMemoryTrain(bool isStart);

    /******************
     *主动训练控制模式
     *参数@int8_t mode   1-位置  2-力量
     *返回值:void
     *****************/
    void switchControlMode(int8_t mode);

    //位置模式回到初始位置
    void rebackToOriginalPosition();

    //运动提示
    void moveTipsCmd(int index,int8_t direction);

    //设置记忆训练等级
    void sendTrainLevel(int8_t level);

    //控制矩阵显示颜色
    void setMatrixColor(int index,E_Color color);

    //生成num个随机数
    QList<uint8_t> generateRandomNum(int num);

    //记忆训练小流程
    void memeoryTrainProcess();

    //根据待训练手指给出总提示
    void giveTotalMoveTips(const QList<uint8_t>&);

    //结束当前流程
    void stopCurrentProcess(bool isRight);

    //重置倒计时
    void resetTimeOutTimer();

    //重置矩阵显示
    void resetMatrix();

    //设置手指的使能状态
    void updateFingersEnableState(int8_t*);

    //设置测试控件的显隐
    void setTestVisible(bool isShow);
private:
    Ui::MemoryTraining *ui;

    QStringList levelNormalList;
    QStringList levelSelectedList;

    QPair<QString,QString> bucklingTypeStyle; //<normal,selected>
    QPair<QString,QString> stretchTypeStyle;
    QPair<QString,QString> circleLockStyle;

    int m_handType;             //左右手
    ControlSet *m_controlSet;
    bool m_isTrainning;

    ST_DestPosition st_bucklingPosition;
    ST_DestPosition st_stretchPosition;

    ST_RealTimeFingersPosition st_currentFingerPosition;
    ST_RealTimeStrength st_currentFingerStrength;

    int m_currentFingerIndex;   //当前手指
    int m_currentMode;          //1-力量模式  2-位置模式
    int m_currentDirection;     //当前运动方向 1-伸展（向上 力量为负值）  -1-屈曲（向下 力量为正直）

    QTimer *m_timeOutTimer;     //超时检测定时器
    QTimer *m_forceTimer;       //力量检测
    QTimer *m_positionTimer;    //位置检测

    //超时检测
    int m_theRestTime;      //倒计时计时
    int currentTime;        //当前所用时间

    int8_t fingerEnable[5]; //手指使能状态
    QList<QLabel*> lableList;
    bool m_isLoop;  //是否循环

    //记忆训练流程
    int m_lastProcessNum;                 //记忆的最大魔方数
    int m_currentProcessTrainFingerNum;   //当前小流程要训练的手指数,取值范围1~20
    int m_currentProcessIndex;            //当前小流程m_currentProcessToTrainFinger的下标
    QList<uint8_t> m_randomList;          //待训练手指链表（集合）随机生成
    QList<uint8_t> m_currentProcessToTrainFinger;//当前流程待训练手指

    bool m_visualTips;//视觉提示
    bool m_moveTips;//运动提示

    QDateTime startTime;
    QDateTime endTime;

    ST_TrainReport st_trainReport; //训练报告
};

#endif // MEMORYTRAINING_H
