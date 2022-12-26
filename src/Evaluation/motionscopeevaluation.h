 #ifndef MOTIONSCOPEEVALUATION_H
#define MOTIONSCOPEEVALUATION_H

#include <QWidget>
#include "userdataformate.h"
namespace Ui {
class MotionScopeEvaluation;
}

class MotionScopeEvaluation : public QWidget
{
    Q_OBJECT

public:
    explicit MotionScopeEvaluation(QWidget *parent = nullptr);
    ~MotionScopeEvaluation();
public slots:
    void receiveCommData(QByteArray);

    //手指状态发生变化
    void slotFingerStateChanged(int8_t index,int8_t state);

    //界面发生变化
    void slotCurrentPageChanged(int index);

    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);

    //当前用户改变
    void slotCurrentUserChanged();
protected:
    void showEvent(QShowEvent *event);

private:
    void initWidget();

    /************************************
    *说明:设置各手指的具体位置
    *参数：
    *@ST_FingersPosition st_FingerPosition
    *返回值：无
    ***********************************/
    void setHandFingersPosition(ST_RealTimeFingersPosition st_FingerPosition);

    //计算关节活动度结果
    void calMotionScope();

    //存储关节活动度评定结果
    void saveMotionScopeResult();

    //初始化手指
    void initHandFingers();

    //开始关节活动度评估
    void startMotionRangeEvaluate();

    //清除设置和参数
    void clearSetAndParam();

    //结束关节活动度评估
    void stopMotionRangeEvaluate();
private slots:
    void on_fourFinger_Btn_clicked();

    void on_oneFinger_Btn_clicked();
    //伸展力发生变化
    void on_stretch_Dial_valueChanged(int value);
    //屈曲力发生变化
    void on_buckking_Dial_valueChanged(int value);

    void on_finish_Btn_clicked();

    void on_reEvaluat_Btn_clicked();

    //获取图片名字
    QString getImageName();
private:
    Ui::MotionScopeEvaluation *ui;
    QString boostFourNormal;
    QString boostFourSelected;
    QString boostOneNormal;
    QString boostOneSelected;

    ST_HandData st_HandData;    //当前用户手数据
    QDateTime m_StartDateTime;  //开始评估时间
    QDateTime endTime;
    ST_TrainReport st_trainReport; //训练报告

    int8_t m_evaluateType;  //1-四指评定  2-拇指评定

    //手指的原始状态
    int originalSteate[5];
    //手指当前状态
    int fingersState[5];    //0-disEnable 1-Enable
};

#endif // MOTIONSCOPEEVALUATION_H
