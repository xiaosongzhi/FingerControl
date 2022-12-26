#ifndef ACTIVETRAINING_H
#define ACTIVETRAINING_H
#include "dataFormate.h"
#include <QWidget>
#include "controlset.h"
#include <QTimer>
#include <QDateTime>
#include "userdataformate.h"

#define BASEFORCE 50

namespace Ui {
class ActiveTraining;
}

class ActiveTraining : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveTraining(QWidget *parent = nullptr);
    ~ActiveTraining();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void slotFingerPressed(QString fingerName);

    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);

    //用户切换
    void slotCurrentUserChanged();
private slots:
    void receiveCommData(QByteArray);

    void on_level1_Btn_clicked();

    void on_level2_Btn_clicked();

    void on_level3_Btn_clicked();

    void on_bucklingType_Btn_clicked();

    void on_stretchType_Btn_clicked();

    void on_randomType_Btn_clicked();

    void on_manualType_Btn_clicked();

    void on_autoType_Btn_clicked();

    void on_controlSet_Btn_clicked();

    void on_display_Btn_clicked();

    void on_start_Btn_clicked();

    void on_finish_Btn_clicked();

    void on_timeLimit_SpinBox_valueChanged(int arg1);

    void slotTimerOut();

    //力量模式下判断运动是否正确
    void slotChargeForceDirection();

    //位置模式下判断是否正确
    void slotChargePosition();
private:

    void initWidget();

    // 主动训练流程
    void autoTrainProcess();
    /****************
    *发送给下位机预运动
    *参数：@ST_SetFingerPosiotion st_MovePosition 关节序号以及运动方向
    * st_MovePosition
    * 返回值：void
    * **********/
    void sendMoveDirectionCmd(ST_SetFingerPosiotion st_MovePosition);

    //运动提示
    void moveTipsCmd(int index,int8_t direction);

    /******************
     * 发送主动训练难度等级
     *参数@int8_t level  1,2,3难度等级
     *返回值:void
     * ****************/
    void sendTrainLevel(int8_t level);

    /******************
     * 告知下位机开始/结束主动训练
     *参数@bool isStart   true-开始训练  false-结束训练
     *返回值:void
     * ****************/
    void switchActiveTrain(bool isStart);

    /******************
     *主动训练控制模式
     *参数@int8_t mode   1-力量模式  2-位置模式
     *返回值:void
     *****************/
    void switchControlMode(int8_t mode);

    //位置模式回到初始位置
    void rebackToOriginalPosition();

    //设置各手指的位置
    void setHandFingersPosition(ST_HandFingersPosition&);

    //设置手指的使能状态
    void updateFingersEnableState(int8_t*);

    void setTestVisible(bool isShow);
private:
    Ui::ActiveTraining *ui;

    //UI相关
    QStringList levelNormalList;
    QStringList levelSelectedList;
    QPair<QString,QString> bucklingTypeStyle; //<normal,selected>
    QPair<QString,QString> stretchTypeStyle;
    QPair<QString,QString> randomTypeStyle;
    QPair<QString,QString> manualTypeStyle;
    QPair<QString,QString> autoTypeStyle;
    ControlSet *m_controlSet;
    //训练相关
    struct //训练类型
    {
        int8_t upDown;      //-1-down 1-up 2-random
        bool isAuto;        //false-鼠标选择 true-自动
    }st_TrainParam;

    int m_currentMode;      //1-力量模式  2-位置模式
    int m_currentDirection; //当前运动方向 1-伸展（向上 力量为负值）  -1-屈曲（向下 力量为正直）

    QTimer *m_timeOutTimer; //超时检测定时器
    QTimer *m_forceTimer;     //力量检测
    QTimer *m_positionTimer;  //位置检测

    int m_theRestTime;      //倒计时计时
    int m_rightTimes;       //训练总次数
    int m_errorTimes;       //总错误次数
    int m_timeOutTimes;     //超时次数
    int m_handType;         //左右手

    bool m_isTrainning;     //训练状态
    int m_currentFingerIndex;//当前手指
    int m_forceLevel;   //力量等级

    int8_t fingerEnable[5]; //手指使能状态

    ST_RealTimeFingersPosition st_currentFingerPosition;
    ST_RealTimeStrength st_currentFingerStrength;

    int currentTime; //超时数据

    ST_DestPosition st_bucklingPosition;
    ST_DestPosition st_stretchPosition;

    QDateTime startTime;
    QDateTime endTime;

    ST_TrainReport st_trainReport; //训练报告

    bool m_isDisplayOn;   //是否打开颜色提示

    bool testDirection; //测试方向
};

#endif // ACTIVETRAINING_H
