#ifndef TRAININGHISTORY_H
#define TRAININGHISTORY_H

#include <QWidget>
#include "userdataformate.h"
#include "reportforms.h"
#include <QLabel>
class ReportDialog;

namespace Ui {
class TrainingHistory;
}

class TrainingHistory : public QWidget
{
    Q_OBJECT

public:
    explicit TrainingHistory(QWidget *parent = nullptr);
    ~TrainingHistory();

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_previous_Btn_clicked();

    void on_next_Btn_clicked();
    //查看按钮
    void slotViewBtn();
    //导出按钮
    void slotImportBtn();
    //当前用户发生改变
    void slotCurrentUserChanged();
    //打印
    void on_printReport_Btn_clicked();

    void on_tipConfirm_Btn_clicked();

private:
    //设置tableWidget的属性
    void initTableWidget();
    //在表中填充数据
    void fillTableWidget(ST_TrainReport&,int row);

    /***设置第n行按钮的显隐**
     * @ int row 行数，从0开始， -1代表所有行
     * @ bool hide, true-隐藏  false-显示
     * *********/
    void setRowButtonHide(int row,bool hide);

    //刷新表格
    void flushTableData();

    //计算各种训练的时长
    int calculateTrainDurationTime(int* data);

    //获取力量评估历史数据
    int getForceEvaluationData(ReportDataList &reportDataList);

    //获取范围评估历史数据
    int getScopeEvaluationData(ReportData &reportData);
private:
    Ui::TrainingHistory *ui;
    int m_currentPage;          //tablewidget当前的页码,从1开始
    const int m_perPageCount;   //每页显示的个数
    QList<QVariantMap> m_historyListMap;
    int m_maxRow;               //最大行数暂定设为100
    int m_totalPage;            //总页数
    int m_totalReportNum;       //报告的总条数
    ReportForms *m_reportDialog;//报表
    QLabel resultLabel;
};

#endif // TRAININGHISTORY_H
