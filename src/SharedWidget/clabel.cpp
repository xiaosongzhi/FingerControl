#include "clabel.h"
#include <QMouseEvent>
#include <QDebug>
#include "dataFormate.h"
CLabel::CLabel(QWidget *parent) :
    QLabel(parent),
    isPressedOn(true),
    isTurnOnDelayShow(false)
{
}

void  CLabel::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    if(isPressedOn)
    {
        emit signalOnPressed(m_name);
    }
}

void CLabel::setLabelName(QString name)
{
    m_name = name;
}
QString CLabel::getLableName()
{
    return m_name;
}

void CLabel::switchPressend(bool isOn)
{
    isPressedOn = isOn;
}

void CLabel::setMyStyleSheet(QString styleSheet)
{
    setStyleSheet(styleSheet);
    if(isTurnOnDelayShow)
    {
        Sleep(1000);
        setStyleSheet("");
    }
}

void CLabel::switchDelayShow(bool on)
{
    isTurnOnDelayShow = on;
}
