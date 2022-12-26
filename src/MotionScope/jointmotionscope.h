#ifndef JOINTMOTIONSCOPE_H
#define JOINTMOTIONSCOPE_H

/***********关节活动度设定界面类*************/

#include <QWidget>
#include "userdataformate.h"
#include "machinecheckdialog.h"
#include "warningdialog.h"
namespace Ui {
class JointMotionScope;
}
class QButtonGroup;
class JointMotionScope : public QWidget
{
    Q_OBJECT

public:
    explicit JointMotionScope(QWidget *parent = nullptr);
    ~JointMotionScope();
    //切换左右手
    void switchHand(int type);

    void initWidget();

    /************************************
    *说明:设置手指（包括与手指相关的按钮）使能/禁用
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@bool isEnable：true-正常 false-禁用
    *返回值：无
    ********************************/
    void setFingerEnable(int handType,int fingerIndex,bool isEnable);

    //设置用户手指状态数据
    void setFingersState(ST_HandData st_Hand);

    /************************************
    *说明:设置各手指的具体位置
    *参数：
    *@ST_FingersPosition st_FingerPosition
    *返回值：无
    ***********************************/
    void setHandFingersPosition(ST_RealTimeFingersPosition st_FingerPosition);

protected:
    virtual void showEvent(QShowEvent *event);

    virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_reset_Btn_clicked();

    void on_save_Btn_clicked();

    void on_allOut_Btn_clicked();

    void on_fourInOneOut_Btn_clicked();

    void on_fourOutOneIn_Btn_clicked();

    void slotBtnGroupClicked(int btnId);

    void slotResetBtnGroupClicked(int btnId);

    void on_L_Btn_clicked();

    void on_R_Btn_clicked();

    void on_lock_Btn_clicked();

    void slotCurrentUserChanged();

    void slotWarningType(E_WarningType);
public slots:
    void receiveCommData(QByteArray);
    //手指上下箭头的控制
    void slotFingerValueChanged(int handType,int fingerIndex,int upDown);

    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);
private:
    /************************************
    *说明:设置reset按钮 使能/禁用
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@bool isEnable：true-正常 false-禁用
    *返回值：无
    ********************************/
    void setResetBtnEnable(int handType,int fingerIndex,bool isEnable);

    //进入机械校准界面
    void enterMachineCheckPage();

    //发送手以及手指数据
    void setHandTypeAndFinger(ST_HandData);

    /************机械校准指令************
    *参数：
    * @uint8_t type 1-四指 2-拇指
    *******/
    void machineCheck(uint8_t type);

    //将手指的活动范围发送给下位机
    void sendFingersRangeToSlave(QMap<int,QPair<int,int>>&);

    //开始测试
    void resetTrainTest();
    //设置测试控件的显隐
    void setTestVisible(bool isShow);
    //手指接触风险显示 type 0-左 1-右
    void setTouchVisible(int type,bool isVisible);

private:
    Ui::JointMotionScope *ui;
    QButtonGroup *btnGroupBox;

    QButtonGroup *resetBtnGroup;

    int m_handType; //左右手标志位

    int m_currentID;//当前用户ID

    ST_HandData st_HandData;//当前用户手数据

    WarningDialog *m_MachineCheckDialog;//机械校准提示框

    WarningDialog *m_MoveTipDialog; //移动前提示框

    WarningDialog* m_touchTipDialog;//接触风险

    bool isTesting; //是否正在测试的标志

    bool m_isMachineChecked; //是否进行机械校准
};

#endif // JOINTMOTIONSCOPE_H
