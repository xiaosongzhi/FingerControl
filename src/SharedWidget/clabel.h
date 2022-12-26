#ifndef CLABEL_H
#define CLABEL_H

#include <QWidget>
#include <QLabel>
class CLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CLabel(QWidget *parent = nullptr);

    void setLabelName(QString);
    QString getLableName();

    void switchPressend(bool);

    void setMyStyleSheet(QString);

    void switchDelayShow(bool on);
signals:
    void signalOnPressed(QString);
protected:
    virtual void  mousePressEvent(QMouseEvent *ev) override;
private:
    QString m_name;
    bool isPressedOn;   //开关接收鼠标事件
    bool isTurnOnDelayShow;//是否打开延时显示，在主动运动时需打开，初始化时需关闭
};

#endif // CLABEL_H
