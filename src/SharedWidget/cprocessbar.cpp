#include "cprocessbar.h"
#include <QPainter>
#include <QDebug>
CProcessBar::CProcessBar(QWidget *parent) : QWidget(parent),
    moveRectColor("#0D9DDB"),
    m_width(2),
    m_height(2),
    isLineShow(false),
    m_direction(0),
    m_isEnabled(true),
    m_isScopeRectShow(false),
    isPassive(false)
{
    lineTimer = new QTimer(this);
    connect(lineTimer,SIGNAL(timeout()),this,SLOT(slotLineTimer()));
    lineTimer->setSingleShot(true);
    lineTimer->setInterval(3000);
}

CProcessBar::CProcessBar(QColor borderColor,QColor backColor,QSize size)
{
    Q_UNUSED(borderColor)
    Q_UNUSED(backColor)
    resize(size);
}

void CProcessBar::setDirection(int type) //0-水平 1-竖直
{
    m_direction = type;
    if(0 == type)
    {
        //10
        moveRect.setRect(10,2,40,12);
        markRect.setRect(10,0,2,16);
    }
    else if(1 == type)
    {
        //10
        moveRect.setRect(2,10,12,40);
        markRect.setRect(0,0,16,2);
    }
    update();
}

void CProcessBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    QPen pen;
    pen.setColor("#D2E9F3");
    pen.setWidth(0);

    painter.setPen(pen);
    //设置反走样
    painter.setRenderHint(QPainter::Antialiasing,true);

    //水平方向
    if(0 == m_direction)
    {
        //绘制基础图形
        QBrush brush("#D2E9F3");
        if(!m_isEnabled)
            brush.setColor("#CCCCCC");
        if(isPassive)
            brush.setColor("#888888");

        painter.setBrush(brush);
        QRect rect(0,2,width(),height()-4);
        painter.drawRoundedRect(rect,6,6);

        //绘制关节活动范围矩形
        QBrush brushScope("#A2E9F3");
        if(m_isScopeRectShow)
        {
            painter.setBrush(brushScope);
            painter.drawRoundedRect(baseScopeRect,6,6);
        }
        //绘制移动矩形
        QBrush Movebrush(moveRectColor);
        painter.setBrush(Movebrush);
        moveRect.setHeight(height()-4);
        if(m_isEnabled)
            painter.drawRoundedRect(moveRect,6,6);

        if(isLineShow)
        {
            QBrush markBrush(Qt::white);
            painter.setBrush(markBrush);
            painter.drawRect(markRect);
        }
    }
    else if(1 == m_direction) //竖直方向
    {
        //绘制基础图形
        QBrush brush("#D2E9F3");
        if(!m_isEnabled)
            brush.setColor("#CCCCCC");
        if(isPassive)
            brush.setColor("#888888");
        painter.setBrush(brush);
        QRect rect(2,0,width()-4,height());

        painter.drawRoundedRect(rect,6,6);

        //绘制关节活动范围矩形
        QBrush brushScope("#A2E9F3");
        if(m_isScopeRectShow)
        {
            painter.setBrush(brushScope);
            painter.drawRoundedRect(baseScopeRect,6,6);
        }
        //绘制移动矩形
        QBrush Movebrush(moveRectColor);
        painter.setBrush(Movebrush);
        moveRect.setWidth(width()-4);

        if(m_isEnabled)
        {
            painter.drawRoundedRect(moveRect,6,6);
        }
        if(isLineShow)
        {
            QBrush markBrush(Qt::white);
            painter.setBrush(markBrush);
            painter.drawRect(markRect);
        }
    }
}

void CProcessBar::setMoveRect(QRect rect)        //设置移动矩形的大小和位置
{
    moveRect = rect;
    update();
}
void CProcessBar::setMoveRectColor(QColor color)  //设置移动矩形的颜色
{
    moveRectColor = color;
    update();
}
//实时位置
void CProcessBar::setMark(uint8_t position)  //相对位置
{
    //水平方向
    if(0 == m_direction)
    {
        //改变显示位置2021.9.18
        int x =width()*(position)/100.f;
        markLine.setLine(x,0,x,height());
        markRect.setTopLeft(QPoint(x,0));
        markRect.setWidth(2);
    }
    else if(1 == m_direction)
    {
        int y = height()*position/100.f;
        markLine.setLine(2,y,width(),y);
        markRect.setTopLeft(QPoint(0,y));
        markRect.setHeight(2);
    }
    isLineShow = true;
    update();
}

void CProcessBar::setMarkLine(uint8_t position)
{
    //水平方向
    if(0 == m_direction)
    {
        //改变显示位置2021.9.18
        int x =width()*(position)/100.f;
        markLine.setLine(x,0,x,height());
        markRect.setTopLeft(QPoint(x,0));
        markRect.setWidth(2);
    }
    else if(1 == m_direction)
    {
        int y = height()*position/100.f;
        markLine.setLine(2,y,width(),y);
        markRect.setTopLeft(QPoint(0,y));
        markRect.setHeight(2);
    }
    isLineShow = true;
    update();
}

//position为0~100之间的一个数
void CProcessBar::setMoveRectPosition(uint8_t position)//设置矩形相对位置
{
    if(0 == m_direction)
    {
        int x = width()*position/100.f-moveRect.width()/2;
        int y = 2;
        moveRect.setTopLeft(QPoint(x,y));
        moveRect.setWidth(m_width);

    }
    else if(1 == m_direction)
    {
        int x = 2;
        int y = height()*position/100.f-moveRect.height()/2;
        moveRect.setTopLeft(QPoint(x,y));
        moveRect.setHeight(m_height);

    }

    update();
}

void CProcessBar::setMoveRectMinMargin()
{
    if(0 == m_direction)
    {
        moveRect.setTopLeft(QPoint(markLine.p1().x(),2));
        m_width = qAbs(moveRect.left() - moveRect.right());
    }
    else if(1 == m_direction)
    {
        moveRect.setTopLeft(QPoint(markLine.p1().x(),markLine.p1().y()));
        m_height = qAbs(moveRect.top() - moveRect.bottom());
    }

    update();
}

void CProcessBar::setMoveRectMaxMargin()
{
    if(0 == m_direction)
    {
        moveRect.setBottomRight(QPoint(markLine.p1().x(),height()-2));
        m_width = qAbs(moveRect.left() - moveRect.right());
    }
    else if(1 == m_direction)
    {
        moveRect.setBottomRight(QPoint(markLine.p2().x(),markLine.p2().y()));
        m_height = qAbs(moveRect.top() - moveRect.bottom());
    }
    update();
}

void CProcessBar::slotLineTimer()
{
    if(isLineShow)
    {
        isLineShow = false;
        update();
    }
}

void CProcessBar::setLineHide()
{
//    lineTimer->start();
}

void CProcessBar::setFingerEnable(bool isEnable)
{
    m_isEnabled = isEnable;
    update();
}

//重置手指状态
void CProcessBar::resetFingerValue(int value)
{
    if(0 == m_direction)
    {
        moveRect.setRect(value,2,0,12);
        markRect.setRect(value,0,2,16);
    }
    else if(1 == m_direction)
    {
        moveRect.setRect(2,value,12,0);
        markRect.setRect(0,value,16,2);
    }
    update();
}

//设置范围矩形的显隐
void CProcessBar::setScopeRectVisible(bool isShow)
{
    m_isScopeRectShow = isShow;
    update();
}

//0~100之间的数
void CProcessBar::setBaseScopeRect(int min,int max)
{
    //水平
    if(0 == m_direction)
    {
        baseScopeRect.setRect(min*width()/100.f,2,(max-min)*width()/100.f,height()-4);
    }
    //竖直
    else if(1 == m_direction)
    {
        baseScopeRect.setRect(2,min*height()/100.f,width()-4,(max-min)*height()/100.f);
    }
    update();
}

//设置移动活动范围矩形 0~100之间的数
void CProcessBar::setMoveScopeRect(int min,int max)
{
    //水平
    if(0 == m_direction)
    {
        moveRect.setRect(min*width()/100.f,2,(max-min)*width()/100.f,height()-4);
    }
    //竖直
    else if(1 == m_direction)
    {
        moveRect.setRect(2,min*height()/100.f,width()-4,(max-min)*height()/100.f);
    }
    update();
}

void CProcessBar::switchPassive(bool isOn)
{
    isPassive = isOn;
    update();
}
