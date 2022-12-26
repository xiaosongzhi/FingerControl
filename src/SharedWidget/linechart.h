#ifndef LINECHART_H
#define LINECHART_H

#include <QWidget>
#include "dataFormate.h"
#include <QAbstractAxis>
#include <QChart>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class LineChart; }
QT_END_NAMESPACE
using namespace QtCharts;

class LineChart : public QWidget
{
    Q_OBJECT

public:
    LineChart(QWidget *parent = nullptr);
    ~LineChart();
    //设置表格类型
    void initChart(E_ChartType type,QString title);
    //给图表添加数据
    void setChartData(E_ChartType type,DataTable);
    //设置坐标的取值范围
    void setAxisValue(E_AxisType axis,float minValue,float maxValue);
    //获取坐标轴的属性，由此来设置坐标轴
    QAbstractAxis *getAxisProperties(E_AxisType axis);
    //设置背景主题
    void setWidgetTheme(QChart::ChartTheme theme);
private:
    void createAreaChart(DataTable chartDataTable) const;
    void createBarChart(DataTable chartDataTable) const;
    void createPieChart(DataTable chartDataTable) const;
    void createLineChart(DataTable chartDataTable) const;
    void createSplineChart(DataTable chartDataTable) const;
    void createScatterChart(DataTable chartDataTable) const;
private:
    Ui::LineChart *ui;

    int m_listCount;
    int m_valueMax;
    int m_valueCount;

    QChartView *m_chartView;
    QChart *m_chart;

};
#endif // LINECHART_H
