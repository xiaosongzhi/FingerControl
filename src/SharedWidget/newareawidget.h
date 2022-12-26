#ifndef NEWAREAWIDGET_H
#define NEWAREAWIDGET_H

#include <QWidget>
#include <QChart>
#include "dataFormate.h"
#include <QLineSeries>
#include <QValueAxis>
#include <QAreaSeries>
using namespace QtCharts;
namespace Ui {
class NewAreaWidget;
}

class NewAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewAreaWidget(QWidget *parent = nullptr);
    ~NewAreaWidget();
    //设置四指的实时域图，主要使用sin曲线 -100~100
    void repaintChart(ST_RealTimeStrength st_realStrength);
    //设置拇指的实时域图 取值范围-100~100
    void repaintChart(int16_t thumbValue);
    //清理拇指域图
    void clearThumbChart();
    //清理四指域图
    void clearFourChart();
private:
    //数据转换
    float translateData(int srcMax,int srcMin,int srcCurrent,int destMax,int destMin);
    //数据结构转换
    void structTranslate(ST_RealTimeStrength&);
    //初始化图表
    void initChartData();
private:
    Ui::NewAreaWidget *ui;
    QChart *m_chart;
    QValueAxis *fourAxisY;
    QValueAxis *thumbAxisY;
    //临时域图容器，该指针的作用是为了绘制动态域图是会发生内存泄露的问题
    QAreaSeries *fourLastAreaSeries;
    QAreaSeries *thumbLastAreaSeries;

    int16_t thumbMin,thumbMax;
    int16_t fourMin[4],fourMax[4];
};

#endif // NEWAREAWIDGET_H
