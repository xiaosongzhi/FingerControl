#ifndef CHANDSTATESHOW_H
#define CHANDSTATESHOW_H

#include <QWidget>
#include "dataFormate.h"
namespace Ui {
class CHandStateShow;
}

class CHandStateShow : public QWidget
{
    Q_OBJECT

public:
    explicit CHandStateShow(QWidget *parent = nullptr);
    ~CHandStateShow();
    //设置手指状态
    void setFingerState(int fingerIndex,int state);
    //设置左右手
    void setHandType(int type);
    //设置手指使能与否
    void setFingerEnabled(int index,bool isEnabled);
    //初始化组件
    void initWidget();
    //单个手指延时显示开关
    void switchFingerDelayShow(bool on);
    //是否接收点击事件
    void switchPressedEvent(bool on);
    //根据力量值设置手指颜色
    void setFingerColorByStrength(ST_RealTimeStrength);
    //清除显示效果
    void clearFingerColorEffect();

signals:
    void signalLabelOnPressed(QString);
protected:
    virtual void resizeEvent(QResizeEvent *event);
private:
    Ui::CHandStateShow *ui;
    int m_handType; // 0-left 1-right
    int m_maxValue;
};

#endif // CHANDSTATESHOW_H
