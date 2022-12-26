#ifndef CHANDSCOPEWIDGET_H
#define CHANDSCOPEWIDGET_H

#include <QWidget>
#include <QMap>
#include <userdataformate.h>

//手指关节活动度控件
class QPushButton;
class QButtonGroup;
namespace Ui {
class CHandScopeWidget;
}

class CHandScopeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CHandScopeWidget(QWidget *parent = nullptr);
    ~CHandScopeWidget();

    //切换左右手
    void switchHand(int type);//0-左手 1-右手

    //初始化数据
    void initData();

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
    *说明:禁用或者使能手指
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@bool isEnable：true-可用 false-禁用
    *返回值：无
    ********************************/
    void setHandFingerEnable(int handType,int fingerIndex,bool isEnable);

    //获取各个手指的活动度
    QMap<int,QPair<int,int>> getFingersRange();
    //获取相对活动度
    QMap<int,QPair<int,int>>getOppositeRange();
signals:
    /************************************
    *说明:使用箭头按钮控制手指的位置
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@int upDown：-1向下 1-向上
    *返回值：无
    ********************************/
    void singnalFingerValueChanged(int handType,int fingerIndex,int upDown);

private slots:
    void slotMinusBtnGroup(int);

    void slotPlusBtnGroup(int);

private:
    void initWidget();

    /************************************
    *说明:根据输入的数据，计算手机活动范围
    *参数：
    *@int fingerIndex: 0-4 手指序号
    *@int value: 输入的值
    *@int *minValue: 最小值
    *@int *maxValue: 最大值
    *返回值：无
    ********************************/
    void getFingerValueMargin(int fingerIndex,int value);

    /************************************
    *说明:设置滑条周围按钮的使能/禁用
    *参数：
    *@int handType： 0-左手 1-右手
    *@int fingerIndex：手指序号，拇指-小指 0-4
    *@bool isEnable：true-正常 false-禁用
    *返回值：无
    ********************************/
    void setFingerBtnEnable(int handType,int fingerIndex,bool isEnable);
private:
    Ui::CHandScopeWidget *ui;
    QMap<int,QList<bool>> m_mapHandFingerState;//QMap<左右手,QList<是否可用>> 下标0~4  QList大小为5，保持不变
    int m_handType;

    QMap<int,QPair<int,int>>m_mapFingersRange;   //手指的活动范围 Map大小为5，不管手指是否被选中，map的大小都是5
    QMap<int,QPair<int,int>> m_tempFingerRange;

    QButtonGroup *m_plusGroup;
    QButtonGroup *m_minusGroup;

    QList<QPushButton*> leftHandFingerBtnList;
    QList<QPushButton*> rightHandFingerBtnList;

    int currentPosition;

    int fingersCurrentPosition[5];
};

#endif // CHANDSCOPEWIDGET_H
