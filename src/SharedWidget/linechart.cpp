#include "linechart.h"
#include "ui_linechart.h"
#include <QtCharts>
#include <QGraphicsScene>
#include <QAbstractAxis>
#include <QLayout>
#include <QPointF>
#include <QPalette>

LineChart::LineChart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LineChart)
    ,m_chart(NULL)
{
    ui->setupUi(this);
#if 0
    QChart *chart = new QChart();
    QLineSeries* lineSeries = new QLineSeries();
    lineSeries->append(1,1);
    lineSeries->append(10,20);
    lineSeries->append(25,25);
    lineSeries->append(50,30);
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->append(10,10);
    scatterSeries->append(40,30);
    scatterSeries->append(10,20);
    chart->addSeries(lineSeries);
    chart->addSeries(scatterSeries);
    QValueAxis *xAxis = new QValueAxis();
    QValueAxis *yAxis = new QValueAxis();
    xAxis->setRange(0,100);
    yAxis->setRange(0,50);
    chart->addAxis(xAxis,Qt::AlignLeft);
    chart->addAxis(yAxis,Qt::AlignBottom);
#endif
    m_chartView = new QChartView();
    QLayout *layout = new QGridLayout();
    // 反锯齿渲染
    m_chartView->setRenderHint(QPainter::Antialiasing,true);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    m_chartView->setContentsMargins(0,0,0,0);
    layout->addWidget(m_chartView);
    setLayout(layout);
}

LineChart::~LineChart()
{
    delete ui;
}

//设置表格类型
void LineChart::initChart(E_ChartType type,QString title)
{
    Q_UNUSED(type)

    if(m_chart)
        delete m_chart;

    QChart *chart = new QChart();
//    chart->setTitle(title);

    m_chart = chart;
    m_chart->setMargins(QMargins(0,0,0,0));
}
//给表格添加数据
void LineChart::setChartData(E_ChartType type,DataTable data)
{
    if(!m_chart)
        return;
    if(data.isEmpty())
    {
        qDebug()<<"待绘制数据为空";
        return;
    }
    switch(type)
    {
    case AreaChart_E:
        createAreaChart(data);
        break;
    case BarChart_E:
    {

    }
        break;
    case PieChart_E:
        createPieChart(data);
        break;
    case LineChart_E:
        createLineChart(data);
        break;
    case SplineChart_E:
        createSplineChart(data);
        break;
    case ScatterChart_E:
        break;
    }
}
//设置坐标的取值范围
void LineChart::setAxisValue(E_AxisType axis,float minValue,float maxValue)
{
    if(!m_chart)
        return;
    switch(axis)
    {
    case X_Axis:
    {
        m_chart->axes(Qt::Horizontal).first()->setRange(minValue,maxValue);
        break;
    }
    case Y_Axis:
        m_chart->axes(Qt::Vertical).first()->setRange(minValue,maxValue);
        break;
    }
}
//设置坐标轴的属性
QAbstractAxis * LineChart::getAxisProperties(E_AxisType axis)
{
    if(!m_chart)
        return NULL;
    if(m_chart->axes(Qt::Horizontal).isEmpty())
        return NULL;
    switch(axis)
    {
    case X_Axis:
        return m_chart->axes(Qt::Horizontal).first();
    case Y_Axis:
       return  m_chart->axes(Qt::Vertical).first();
    }
    return NULL;
}
//设置背景主题
void LineChart::setWidgetTheme(QChart::ChartTheme theme)
{
    if(m_chart)
    {
        m_chart->setTheme(theme);

        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight)
        {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeDark)
        {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        }
        else if (theme == QChart::ChartThemeBlueCerulean)
        {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        }
        else if (theme == QChart::ChartThemeBrownSand)
        {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeBlueNcs)
        {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeHighContrast)
        {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        }
        else if (theme == QChart::ChartThemeBlueIcy)
        {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else
        {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }

}

void LineChart::createAreaChart(DataTable chartDataTable) const
{
    //Area的创建需要两条线作为边界
    if(!m_chart)
        return;
    //首先释放图标中的所有数据
    m_chart->removeAllSeries();
    QString name("Series ");

    //当chartDataTable中只有一个line时，lowSeries默认使用x轴
    if(1 == chartDataTable.size())
    {
        int nameIndex = 0;
        const DataList listData = chartDataTable.at(0).first;
        QSplineSeries *lineSeries = new QSplineSeries(m_chart);
        for(const Data& point : listData)
        {
            lineSeries->append(point.first);
        }

        QAreaSeries *area = new QAreaSeries(lineSeries);
        area->setName(name + QString::number(nameIndex));
        nameIndex++;
        m_chart->addSeries(area);
    }
    else
    {
        int nameIndex = 0;
        //多于一个line  两个一组构成一个Area
        QLineSeries *lowerSeries = 0;
        QLineSeries *upperSeries = 0;
        for(int i(0);i<chartDataTable.size();i++)
        {
            if(0 == i % 2)
            {
                lowerSeries = new QSplineSeries(m_chart);
                for(const Data &dataPoint : chartDataTable[i].first)
                    lowerSeries->append(dataPoint.first);
            }
            if(1 == i % 2)
            {
               upperSeries = new QSplineSeries(m_chart);
               for(const Data &dataPoint : chartDataTable[i].first)
                   upperSeries->append(dataPoint.first);
               QAreaSeries *area = new QAreaSeries(upperSeries,lowerSeries);
               area->setName(name + QString::number(nameIndex));
               nameIndex++;
               m_chart->addSeries(area);
            }
        }
    }

    m_chart->createDefaultAxes();
    m_chartView->setChart(m_chart);
}
void LineChart::createBarChart(DataTable chartDataTable) const
{
    Q_UNUSED(chartDataTable)
    //根据具体的需求进行更改,以下是具体的类型
    //QBarSeries, QHorizontalBarSeries, QHorizontalPercentBarSeries, QHorizontalStackedBarSeries, QPercentBarSeries, and QStackedBarSeries

}
void LineChart::createPieChart(DataTable chartDataTable) const
{
    if(!m_chart)
        return;
    //首先释放图标中的所有数据
    m_chart->removeAllSeries();
    QString name("Series ");
    QPieSeries *series = new QPieSeries(m_chart);
    for(const Data &data : chartDataTable[0].first)
    {
        QPieSlice *slice = series->append(data.second,data.first.x());
        if (/*data == chartDataTable[0].first()*/1) {
            // Show the first slice exploded with label
            slice->setLabelVisible();
            slice->setExploded();
            slice->setExplodeDistanceFactor(0.5);
        }
    }
    series->setPieSize(0.9);
    m_chart->addSeries(series);
    m_chartView->setChart(m_chart);
}
void LineChart::createLineChart(DataTable chartDataTable) const
{
    if(!m_chart)
        return;
    //首先释放图标中的所有数据
    m_chart->removeAllSeries();
    QString name("Series ");

    int nameIndex = 0;
    for(const DataPair &list : chartDataTable)
    {
        QLineSeries *series = new QLineSeries(m_chart);
        series->setColor(list.second);


        for(const Data &data : list.first)
        {
            series->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        m_chart->addSeries(series);
    }

    m_chart->legend()->hide();
    m_chart->createDefaultAxes();
    m_chartView->setChart(m_chart);
}
void LineChart::createSplineChart(DataTable chartDataTable) const
{
    if(!m_chart)
        return;
    //首先释放图标中的所有数据
    m_chart->removeAllSeries();
    QString name("Series ");

    int nameIndex = 0;
    for(const DataPair &list : chartDataTable)
    {
        QSplineSeries *series = new QSplineSeries(m_chart);
        QPen seriesPen;
        seriesPen.setWidth(2);
        seriesPen.setColor(list.second);
        series->setPen(seriesPen);

        for(const Data &data : list.first)
        {
            series->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        m_chart->addSeries(series);
    }
    m_chart->legend()->hide();
    m_chart->createDefaultAxes();
    m_chartView->setChart(m_chart);
}
void LineChart::createScatterChart(DataTable chartDataTable) const
{
    if(!m_chart)
        return;
    //首先释放图标中的所有数据
    m_chart->removeAllSeries();
    QString name("Series ");
    int nameIndex = 0;
    for (const DataPair &list : chartDataTable) {
        QScatterSeries *series = new QScatterSeries(m_chart);
        for (const Data &data : list.first)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        m_chart->addSeries(series);
    }
    m_chart->createDefaultAxes();
    m_chartView->setChart(m_chart);
}
