#ifndef TABLECONTENT_H
#define TABLECONTENT_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include "dataFormate.h"

namespace Ui {
class tableContent;
}

using namespace QtCharts;

class tableContent : public QWidget
{
    Q_OBJECT

public:
    explicit tableContent(QWidget *parent = nullptr);
    ~tableContent();

    //设置力量曲线
    void setForeChart(const ReportDataList&);
    //设置范围曲线
    void setScopeChart(const ReportData&);

    /*****************
     * 函数说明：设置历史运动时间曲线
     * 参数 @const int* data 实际数据的地址
     *     @int size 实际数据的个数
     * 返回值 void
    *****************/
    void setHistoryTimeChart(const int* data,int size);

    void setForceChartVisible(bool visible);
    void setTimeChartVisible(bool visible);
    void setScopeChartVisible(bool visible);
private:
    void initChart();

private:
    Ui::tableContent *ui;
    QChart *m_timeChart;
    QChart *m_scopeChart;
    QChart *m_forceChart;
};

#endif // TABLECONTENT_H
