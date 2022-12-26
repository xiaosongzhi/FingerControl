#ifndef CTRAININGHAND_H
#define CTRAININGHAND_H
#include <QMap>
#include <QWidget>
#include "dataFormate.h"

#define CONDITION  0

class QButtonGroup;
class CProcessBar;
namespace Ui {
class CTrainingHand;
}

/*******关节活动度评估类********
 * 说明：关节活动度评估不受之前选择的手指的影响
 * 也就说，关节活动度所有手指都可以评估，并且可以随时打开关闭某个需要评估的手指
 * 并不是只有右问题的手指才参加评估，好的手指也可以参加评估
**************/

class CTrainingHand : public QWidget
{
    Q_OBJECT

public:
    explicit CTrainingHand(QWidget *parent = nullptr);
    ~CTrainingHand();

    //初始化数据
    void initData();

    //切换左右手 0-左手  1-右手
    void switchHand(int handType);

    /************************************
    *说明:设置各手指的具体值
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@int value：具体位置的百分比 0~100
    *返回值：无
    ***********************************/
    void setHandFingerValue(int handType,int fingerIndex,int value);

    //重置各个手指
    void resetHandFingerValue(int handType,int fingerIndex);

    /************************************
    *说明:禁用或者使能手指（在调用该插件时需要提前设置）
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@bool isEnable：true-可用 false-禁用
    *返回值：无
    ********************************/
    void setHandFingerEnable(int handType,int fingerIndex,bool isEnable);

    //获取各个手指的活动度
    QMap<int,QPair<int,int>> getFingersRange();

    //设置模式,不同的模式初始化成不同的状态(主要用于被动和助力) 2021.9.14 MHL
    void setMode(E_TRAINMODE);

    //设置基础活动范围，该活动范围从关节活动度界面获取 2021.9.14
    void setBaseMoveScope(const QMap<int,QPair<int,int>>&);

    //设置各个关节活动的实际活动范围 2021.9.14
    void setRealMoveScope(const QMap<int,QPair<int,int>>&);

    //范围测试开关
    void switchScope(bool);
private:
    void initWidget();

    /************************************
    *说明:根据输入的数据，计算手机活动范围
    *参数：
    *@int fingerIndex: 0-4 手指序号
    *@int value: 输入的值
    *返回值：无
    ********************************/
    void setFingerValueMargin(int fingerIndex,int value);


signals:
    //手指状态发生变化
    void signalFingerStateChanged(int8_t index,int8_t state);

private slots:
    void slotRightBtnClicked(int);
    void slotLeftBtnClicked(int);

protected:


private:
    Ui::CTrainingHand *ui;
    QButtonGroup *m_LButtonGroup;
    QButtonGroup *m_RButtonGroup;
    QList<CProcessBar *> m_LProcessBar;
    QList<CProcessBar *> m_RProcessBar;
    QMap<int,QList<bool>> m_mapHandFingerState;//QMap<左右手,QList<是否可用>> 下标0~4  QList大小为5，保持不变
    QMap<int,QPair<int,int>>m_mapFingersRange;
    int8_t fingersEnable[5];
    int m_currentPosition;    //手指的当前位置
    int m_handType; //0-左手  1-右手
    bool m_scopeState;  //范围测试开关
};

#endif // CTRAININGHAND_H
