#ifndef CPROCESSBAR_H
#define CPROCESSBAR_H

#include <QWidget>
#include <QTimer>
class CProcessBar : public QWidget
{
    Q_OBJECT
public:
    explicit CProcessBar(QWidget *parent = nullptr);

    CProcessBar(QColor borderColor,QColor backColor,QSize size);

    void setMoveRect(QRect);        //设置移动矩形的大小和位置

    void setMoveRectColor(QColor);  //设置移动矩形的颜色

    void setMoveRectPosition(uint8_t position);//设置矩形相对位置0~100

    void setMark(uint8_t position);  //相对位置0~100

    void setMarkLine(uint8_t position);

    void setMoveRectMinMargin();

    void setMoveRectMaxMargin();

    void setLineHide();
    //设置方向
    void setDirection(int type = 0); //0-水平 1-竖直
    //设置使能与否
    void setFingerEnable(bool isEnable);
    //重置手指状态
    void resetFingerValue(int value);

    //设置范围矩形的显隐(被动模式使用)
    void setScopeRectVisible(bool isShow);
    //设置基础活动范围矩形
    void setBaseScopeRect(int min,int max);
    //被动范围开关
    void switchPassive(bool isOn);
    //设置移动活动范围矩形
    void setMoveScopeRect(int min,int max);
signals:
private slots:
    void slotLineTimer();
protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QRect moveRect;
    QRect baseScopeRect; //范围矩形不会发生变化,其范围直接由关节活动度范围设定时给定（在被动运动中使用）
    QColor moveRectColor;
    QPoint markPosition;
    QLine markLine;
    QRect markRect;     //指示当前位置的线条（只不过是比较粗，所以为矩形）
    int m_width;        //moveRect width
    int m_height;       //moveRect height
    QTimer *lineTimer;
    bool isLineShow;
    int m_direction;    //0-水平 1-竖直
    bool m_isEnabled;
    bool m_isScopeRectShow;//是否显示范围矩形
    bool isPassive; //是否为被动模式
};

#endif // CPROCESSBAR_H
