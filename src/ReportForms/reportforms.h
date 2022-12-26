#ifndef REPORTFORMS_H
#define REPORTFORMS_H
#include <QWidget>
#include "userdataformate.h"
#include <QVBoxLayout>

/********报表生成以及打印模块*****
*使用说明：在使用该模块时只需要简单创建其对象或者是指针然后调用其函数接口即可
* 注意：setHistoryTimeChart、setForeChart、setScopeChart三个函数都需要调用
* 如若不想显示只需要将第一个参数设置为false即可。
* 打印功能随机都可以调用
******************************/

namespace Ui {
class ReportForms;
}
class TableTitle;
class tableContent;
class ReportForms : public QWidget
{
    Q_OBJECT

public:
    explicit ReportForms(QWidget *parent = nullptr);
    ~ReportForms();

    //清除报表头
    void clearReportTitle();
    //填充报表头信息
    void setReportTitleMessage(const ST_PatientMsg &st_patientMsg);

    /********设置报表内容*******/
    /*****************
     * 函数说明：设置历史运动时间曲线
     * 参数  @bool visible 是否可见
     *      @const int* data 实际数据的地址 被动-主动-助力
     *      @int size 实际数据的个数
     * 返回值 void
    *****************/
    void setHistoryTimeChart(bool visible,const int* data,int size);

    //设置力量曲线
    void setForeChart(bool visible,const ReportDataList&);

    //设置范围曲线
    void setScopeChart(bool visible,const ReportData&);

    //打印工程
    void printReport();

    //打印预览
    void printPreview();
private slots:
    void on_prePrint_Btn_clicked();

    void on_print_Btn_clicked();

private:
    void initWidget();

private:
    Ui::ReportForms *ui;
    TableTitle *m_tableTitile;
    tableContent *m_tableContent;
    QVBoxLayout *m_layOut;
};

#endif // REPORTFORMS_H
