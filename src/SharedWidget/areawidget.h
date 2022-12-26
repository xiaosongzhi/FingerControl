#ifndef AREAWIDGET_H
#define AREAWIDGET_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include "dataFormate.h"
#include <QLineSeries>
#include <QValueAxis>
#include <QAreaSeries>

using namespace QtCharts;

class AreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AreaWidget(QWidget *parent = nullptr);

    //设置四指的实时域图，主要使用sin曲线 -100~100
    void repaintChart(ST_RealTimeStrength st_realStrength);

    //设置拇指的实时域图 取值范围-100~100
    void repaintChart(int16_t thumbValue);

    //重置域图
    void resetAreatChart();

    /****设置域图类型
    * 说明:初始化时使用
    * 参数：@int type 0-拇指域图 1-四指域图
    * 返回值:void
    * ***********/
    void setAreaType(int type);
    ~AreaWidget();
signals:
private:
    //数据转换
    float translateData(int srcMax,int srcMin,int srcCurrent,int destMax,int destMin);
    //数据结构转换
    void structTranslate(ST_RealTimeStrength&);
    //初始化图表
    void initChartData();
private:
    QChartView *m_chartView;
    QChart *m_chart;
    QValueAxis *fourAxisY;
    QValueAxis *thumbAxisY;
    //临时域图容器，该指针的作用是为了绘制动态域图是会发生内存泄露的问题
    QAreaSeries *fourLastAreaSeries;
    QAreaSeries *thumbLastAreaSeries;

    int16_t thumbLastValue = 0,thumbMax = 0,thumbMin = 0;

    float max[4] = {0},min[4] = {0};
    short lastPoint[4] = {0};

    int m_areaType;
};

#endif // AREAWIDGET_H
