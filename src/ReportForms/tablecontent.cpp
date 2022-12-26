#include "tablecontent.h"
#include "ui_tablecontent.h"
#include <QLineSeries>
#include <QFormLayout>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QHorizontalBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QDebug>
#include <QPixmap>
tableContent::tableContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tableContent),
    m_timeChart(NULL),
    m_scopeChart(NULL),
    m_forceChart(NULL)
{
    ui->setupUi(this);
    initChart();

    ui->force_Label->setVisible(false);
    ui->scope_Label->setVisible(false);
    ui->time_Label->setVisible(false);
    ui->image_Label->setVisible(false);
}

tableContent::~tableContent()
{
    delete ui;
    delete m_timeChart;
    delete m_scopeChart;
    delete m_forceChart;
}

void tableContent::initChart()
{
    QPixmap pix("./source/image/qushi.png");
    ui->image_Label->setPixmap(pix);

    //设置绘板的反走样
    ui->scope_GraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->force_GraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->time_GraphicsView->setRenderHint(QPainter::Antialiasing);

    m_timeChart = new QChart();
    m_scopeChart = new QChart();
    m_forceChart = new QChart();

    m_timeChart->legend()->setVisible(false);
    m_timeChart->setMargins(QMargins(0,0,0,0));
    ui->time_GraphicsView->setChart(m_timeChart);

    m_scopeChart->legend()->setVisible(false);
    m_scopeChart->setMargins(QMargins(0,0,0,0));
    ui->scope_GraphicsView->setChart(m_scopeChart);

    m_forceChart->legend()->setVisible(false);
    m_forceChart->setMargins(QMargins(0,0,0,0));
    ui->force_GraphicsView->setChart(m_forceChart);
}

//设置力量曲线
void tableContent::setForeChart(const ReportDataList& reportDataList)
{
    //首先清理一下数据
    if(ui->force_GraphicsView->chart()->series().size() > 0)
    {
        ui->force_GraphicsView->chart()->removeAllSeries();
    }
    ui->force_GraphicsView->chart()->removeAxis(m_forceChart->axisX());
    ui->force_GraphicsView->chart()->removeAxis(m_forceChart->axisY());
    //1.准备数据
    //屈曲曲线
    QLineSeries *bucklingLineSeries = new QLineSeries();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    bucklingLineSeries->setPen(pen);
    bucklingLineSeries->setName(tr("屈曲趋向"));
    //伸展曲线
    QLineSeries *stretchLineSeries = new QLineSeries();
    pen.setColor(Qt::green);
    stretchLineSeries->setPen(pen);
    stretchLineSeries->setName(tr("伸展趋向"));

    //2.设置QChart的坐标轴
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy/MM/dd");
    //3.添加数据
    if(reportDataList.size() > 1)
    {
        QMapIterator<QDateTime,int> iter0(reportDataList.at(0));
        while(iter0.hasNext())
        {
            iter0.next();
            bucklingLineSeries->append(iter0.key().toMSecsSinceEpoch(), iter0.value());
        }
        m_forceChart->addSeries(bucklingLineSeries);
        QMapIterator<QDateTime,int> iter1(reportDataList.at(1));
        while(iter1.hasNext())
        {
            iter1.next();
            stretchLineSeries->append(iter1.key().toMSecsSinceEpoch(), iter1.value());
        }
        m_forceChart->addSeries(stretchLineSeries);
    }

    ui->force_GraphicsView->chart()->setAxisX(axisX,bucklingLineSeries);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("N");
    ui->force_GraphicsView->chart()->addAxis(axisY,Qt::AlignLeft);
    bucklingLineSeries->attachAxis(axisY);
    stretchLineSeries->attachAxis(axisX);
    ui->force_GraphicsView->chart()->axisY()->setRange(0,100);
}
//设置范围曲线
void tableContent::setScopeChart(const ReportData& reportData)
{
    //首先清理一下数据
    if(ui->scope_GraphicsView->chart()->series().size() > 0)
    {
        ui->scope_GraphicsView->chart()->removeAllSeries();
    }
    ui->scope_GraphicsView->chart()->removeAxis(m_scopeChart->axisX());
    ui->scope_GraphicsView->chart()->removeAxis(m_scopeChart->axisY());
    //1.准备数据
    QLineSeries *lineSeries = new QLineSeries();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    lineSeries->setPen(pen);
    lineSeries->setName(tr("关节活动度"));

    //2.设置QChart的坐标轴
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy/MM/dd");

    QMapIterator<QDateTime,int> iter(reportData);
    //3.添加数据
    while(iter.hasNext())
    {
        iter.next();
        lineSeries->append(iter.key().toMSecsSinceEpoch(), iter.value());
    }
    m_scopeChart->addSeries(lineSeries);

    ui->scope_GraphicsView->chart()->setAxisX(axisX,lineSeries);
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("%");

    ui->scope_GraphicsView->chart()->addAxis(axisY,Qt::AlignLeft);
    lineSeries->attachAxis(axisY);
    ui->scope_GraphicsView->chart()->axisY()->setRange(0,100);
}
//设置历史运动时间曲线
void tableContent::setHistoryTimeChart(const int* data,int size)
{
    //首先清理一下数据
    if(ui->time_GraphicsView->chart()->series().size() > 0)
        ui->time_GraphicsView->chart()->removeAllSeries();
    ui->time_GraphicsView->chart()->removeAxis(m_timeChart->axisX());
    ui->time_GraphicsView->chart()->removeAxis(m_timeChart->axisY());
    //添加数据
    QHorizontalBarSeries *horizontalSeries = new QHorizontalBarSeries();
    QBarSet *set0 = new QBarSet("");
    for(int i  = 0;i < size;i++)
        (*set0).append(data[i]);
    horizontalSeries->append(set0);

    //设置Y坐标轴
    QStringList categories;
    categories <<tr("被动")<<tr("主动")<<tr("助力");
    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    m_timeChart->addAxis(axisY,Qt::AlignLeft);
    //坐标轴与数据关联
    horizontalSeries->attachAxis(axisY);
    //设置X坐标轴
    QValueAxis *axisX = new QValueAxis();
    m_timeChart->addAxis(axisX,Qt::AlignBottom);
    horizontalSeries->attachAxis(axisX);
    axisX->applyNiceNumbers();
//    axisX->setRange(0,50);
    //将数据添加到图表中
    m_timeChart->addSeries(horizontalSeries);
}

void tableContent::setForceChartVisible(bool visible)
{
    ui->force_Label->setVisible(visible);
    ui->image_Label->setVisible(visible);
    ui->force_GroupBox->setVisible(visible);

}
void tableContent::setTimeChartVisible(bool visible)
{
    ui->time_Label->setVisible(visible);
    ui->time_GroupBox->setVisible(visible);
}
void tableContent::setScopeChartVisible(bool visible)
{
    ui->scope_Label->setVisible(visible);
    ui->scope_GroupBox->setVisible(visible);
}
