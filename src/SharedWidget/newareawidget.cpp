#include "newareawidget.h"
#include "ui_newareawidget.h"
#include <QAreaSeries>
#include <QtMath>
#include <QPen>
NewAreaWidget::NewAreaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewAreaWidget),
    fourLastAreaSeries(NULL),
    thumbLastAreaSeries(NULL)
{
    ui->setupUi(this);
    m_chart = new QChart();
    m_chart->setMargins(QMargins(0,0,0,0));
    ui->graphicsView->setChart(m_chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing,true);

    fourAxisY = new QValueAxis();
    thumbAxisY = new QValueAxis();

    thumbMin = 0;
    thumbMax = 0;
    for(int i = 0;i < 4;i++)
    {
        fourMin[i] = 0;
        fourMax[i] = 0;
    }
}

NewAreaWidget::~NewAreaWidget()
{
    if(m_chart)
    {
        delete m_chart;
        m_chart = NULL;
    }
    delete ui;
}

//设置拇指的实时域图
void NewAreaWidget::repaintChart(int16_t thumbValue)
{
    thumbValue = translateData(MAXSTRENGTH,MINSTRENGTH,thumbValue,100,-100);
    //域图上下边界
    static QPointF lowerList[180] = {};
    static QPointF higherList[180] = {};
    //动态曲线
    static QPointF lineList[180] = {};

    static int16_t lasValue = 0/*,thumbMax = 0,thumbMin = 0*/;

    //上边界
    if(thumbMax < thumbValue)
    {
        thumbMax = thumbValue;
        //生成sin曲线
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i);
            float y = thumbMax/100.f*qSin(qDegreesToRadians(float(i)));
            point.setY(y);
            higherList[i] = point;
        }
    }
    //下边界
    if(thumbMin > thumbValue)
    {
        thumbMin = thumbValue;
        //生成sin曲线
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i);
            float y = -thumbMin/100.f*qSin(qDegreesToRadians(float(i+180)));
            point.setY(y);
            lowerList[i] = point;
        }
    }
    //动态曲线
    if(lasValue != thumbValue)
    {
        lasValue = thumbValue;

        if(lasValue > 0)
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i);
                float y = lasValue/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i] = point;
            }
        }
        else
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i);
                float y = lasValue/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i] = point;
            }
        }
    }

    if(m_chart->series().size() > 0)
    {
        if(thumbLastAreaSeries != NULL)
        {
            delete thumbLastAreaSeries->upperSeries();
            delete thumbLastAreaSeries->lowerSeries();

        }
        m_chart->removeAllSeries();
        thumbLastAreaSeries = NULL;
    }

    m_chart->legend()->hide();
    QLineSeries *maxSeries = new QLineSeries;
    QLineSeries *minSeries = new QLineSeries;
    QLineSeries *lineSeries = new QLineSeries;
    QAreaSeries *maxarea = new QAreaSeries(maxSeries,minSeries);
    thumbLastAreaSeries = maxarea;
    QPen pen(Qt::red);
    pen.setWidth(2);
    lineSeries->setPen(pen);
    maxarea->setBrush(QBrush("#ACD5EA"));
    //填充数据
    for(int i =0;i < 180;i++)
    {
        maxSeries->append(higherList[i]);
        minSeries->append(lowerList[i]);
        lineSeries->append(lineList[i]);
    }

    thumbAxisY->setRange(-1,1);
    //向表格中添加数据，后添加的数据在上层显示
    m_chart->addSeries(maxarea);
    m_chart->addSeries(lineSeries);

    //在设置轴范围
    m_chart->setAxisY(thumbAxisY,maxarea);
    m_chart->setAxisX(thumbAxisY,lineSeries);


    thumbAxisY->hide();
}

void NewAreaWidget::structTranslate(ST_RealTimeStrength& st_realStrength)
{
    st_realStrength.thumb = translateData(MAXSTRENGTH,MINSTRENGTH,st_realStrength.thumb,100,-100);
    st_realStrength.forefinger = translateData(MAXSTRENGTH,MINSTRENGTH,st_realStrength.forefinger,100,-100);
    st_realStrength.middlefinger = translateData(MAXSTRENGTH,MINSTRENGTH,st_realStrength.middlefinger,100,-100);
    st_realStrength.ringfinger = translateData(MAXSTRENGTH,MINSTRENGTH,st_realStrength.ringfinger,100,-100);
    st_realStrength.littlefinger = translateData(MAXSTRENGTH,MINSTRENGTH,st_realStrength.littlefinger,100,-100);
}

//绘制四指实时域图
void NewAreaWidget::repaintChart(ST_RealTimeStrength st_realStrength)
{
    //数据范围转换
    structTranslate(st_realStrength);
    int index = 0;
    //上限
    static QPointF lowerList[720] = {};
    //下限
    static QPointF higherList[720] = {};

    //动态曲线数据
    static QPointF lineList[720] = {};

//    static float max[4] = {0},min[4] = {0};

    //动态曲线顶点值
    static short lastPoint[4] = {0};
    //上限值
    //第1段
    if(fourMax[0] < st_realStrength.forefinger)
    {
        fourMax[0] = st_realStrength.forefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = fourMax[0]/100.f*qSin(qDegreesToRadians(float(i)));
            point.setY(y);
            higherList[i+index] = point;

        }
    }
    if(fourMin[0] > st_realStrength.forefinger)
    {
        fourMin[0] = st_realStrength.forefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = -fourMin[0]/100.f*qSin(qDegreesToRadians(float(i+180)));
            point.setY(y);
            lowerList[i+index] = point;
        }
    }

    if(lastPoint[0] != st_realStrength.forefinger)
    {
        lastPoint[0] = st_realStrength.forefinger;
        if(lastPoint[0] > 0)
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[0]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
        else
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[0]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
    }
    index += 180;
    //第二段
    if(fourMax[1] < st_realStrength.middlefinger)
    {
        fourMax[1] = st_realStrength.middlefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = fourMax[1]/100.f*qSin(qDegreesToRadians(float(i)));
            point.setY(y);
            higherList[i+index] = point;
        }
    }
    if(fourMin[1] > st_realStrength.middlefinger)
    {
        fourMin[1] = st_realStrength.middlefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = -fourMin[1]/100.f*qSin(qDegreesToRadians(float(i+180)));
            point.setY(y);
            lowerList[i+index] = point;
        }
    }

    if(lastPoint[1] != st_realStrength.middlefinger)
    {
        lastPoint[1] = st_realStrength.middlefinger;
        if(lastPoint[1] > 0)
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[1]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
        else
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[1]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
    }
    index += 180;
    //第三段
    if(fourMax[2] < st_realStrength.ringfinger)
    {
        fourMax[2] = st_realStrength.ringfinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = fourMax[2]/100.f*qSin(qDegreesToRadians(float(i)));
            point.setY(y);
            higherList[i+index] = point;
        }
    }
    if(fourMin[2] > st_realStrength.ringfinger)
    {
        fourMin[2] = st_realStrength.ringfinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = -fourMin[2]/100.f*qSin(qDegreesToRadians(float(i+180)));
            point.setY(y);
            lowerList[i+index] = point;
        }
    }
    if(lastPoint[2] != st_realStrength.ringfinger)
    {
        lastPoint[2] = st_realStrength.ringfinger;
        if(lastPoint[2] > 0)
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[2]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
        else
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[2]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
    }
    index += 180;
    //第四段
    if(fourMax[3] < st_realStrength.littlefinger)
    {
        fourMax[3] = st_realStrength.littlefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = fourMax[3]/100.f*qSin(qDegreesToRadians(float(i)));
            point.setY(y);
            higherList[i+index] = point;
        }
    }
    if(fourMin[3] > st_realStrength.littlefinger)
    {
        fourMin[3] = st_realStrength.littlefinger;
        for(int i = 0;i < 180;i++)
        {
            QPointF point;
            point.setX(i+index);
            float y = -fourMin[3]/100.f*qSin(qDegreesToRadians(float(i+180)));
            point.setY(y);
            lowerList[i+index] = point;
        }
    }
    if(lastPoint[3] != st_realStrength.littlefinger)
    {
        lastPoint[3] = st_realStrength.littlefinger;
        if(lastPoint[3] > 0)
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[3]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
        else
        {
            for(int i = 0;i < 180;i++)
            {
                QPointF point;
                point.setX(i+index);
                float y = lastPoint[3]/100.f*qSin(qDegreesToRadians(float(i)));
                point.setY(y);
                lineList[i+index] = point;
            }
        }
    }
    if(m_chart->series().size() > 0)
    {
        if(fourLastAreaSeries != NULL)
        {
            delete fourLastAreaSeries->lowerSeries();
            delete fourLastAreaSeries->upperSeries();
        }
        //m_chart->removeSeries(lastAreaSeries);
        m_chart->removeAllSeries();
        fourLastAreaSeries = NULL;
    }

    m_chart->legend()->hide();
    QLineSeries *maxSeries = new QLineSeries;
    QLineSeries *minSeries = new QLineSeries;


    for(int i = 0;i < 720;i++)
    {
        maxSeries->append(higherList[i]);
    }

    for(int i =0;i < 720;i++)
    {
        minSeries->append(lowerList[i]);
    }

    QLineSeries *lineSeries = new QLineSeries;
    QPen pen(Qt::red);
    pen.setWidth(2);
    lineSeries->setPen(pen);
    for(int i = 0;i < 720;i++)
    {
        lineSeries->append(lineList[i]);
    }
    QAreaSeries *maxarea = new QAreaSeries(maxSeries,minSeries);
    fourLastAreaSeries = maxarea;
    maxarea->setBrush(QBrush("#ACD5EA"));

    fourAxisY->setRange(-1,1);
    //向表格中添加数据，后添加的数据在上层显示
    m_chart->addSeries(maxarea);
    m_chart->addSeries(lineSeries);

    //在设置轴范围
    m_chart->setAxisY(fourAxisY,maxarea);
    m_chart->setAxisX(fourAxisY,lineSeries);
    fourAxisY->hide();
}

float NewAreaWidget::translateData(int srcMax,int srcMin,int srcCurrent,int destMax,int destMin)
{
    float percent = ((float)(srcCurrent - srcMin))/((float)(srcMax-srcMin));
    return (destMax-destMin)*percent - destMax;
}

//清理画布
void NewAreaWidget::clearThumbChart()
{
    thumbMin = 0;
    thumbMax = 0;
    repaintChart(0);
}

void NewAreaWidget::clearFourChart()
{
    for(int i = 0;i < 4;i++)
    {
        fourMin[i] = 0;
        fourMax[i] = 0;
    }
    ST_RealTimeStrength st_realStrength;
    st_realStrength.forefinger = 0;
    st_realStrength.littlefinger = 0;
    st_realStrength.middlefinger = 0;
    st_realStrength.ringfinger = 0;
    repaintChart(st_realStrength);
}
