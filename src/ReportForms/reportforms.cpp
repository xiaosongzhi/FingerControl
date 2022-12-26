#include "reportforms.h"
#include "ui_reportforms.h"
#include "tablecontent.h"
#include "tabletitle.h"
#include "cprinter.h"
ReportForms::ReportForms(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportForms),
    m_tableTitile(NULL),
    m_tableContent(NULL),
    m_layOut(NULL)
{
    ui->setupUi(this);
    this->move((1920-width())/2,(1080-height())/2);
    setWindowTitle("训练报告");
    initWidget();
}

ReportForms::~ReportForms()
{
    delete ui;
    delete  m_tableTitile;
    delete  m_tableContent;
    delete  m_layOut;
}

void ReportForms::initWidget()
{
    m_tableContent = new tableContent();
    m_tableTitile = new TableTitle();
    m_layOut = new QVBoxLayout();
    m_layOut->addWidget(m_tableTitile);
    m_layOut->addWidget(m_tableContent);
    ui->scrollArea->widget()->setLayout(m_layOut);
}

//清除报表头
void ReportForms::clearReportTitle()
{
    m_tableTitile->clearTitle();
}
//填充报表头信息
void ReportForms::setReportTitleMessage(const ST_PatientMsg &st_patientMsg)
{
    m_tableTitile->setTitleMessage(st_patientMsg);
}

void ReportForms::setHistoryTimeChart(bool visible,const int* data,int size)
{
    m_tableContent->setTimeChartVisible(visible);
    if(visible)
    {
        m_tableContent->setHistoryTimeChart(data,size);
    }
}

//设置力量曲线
void ReportForms::setForeChart(bool visible,const ReportDataList& reportDataList)
{
    m_tableContent->setForceChartVisible(visible);
    if(visible)
    {
        m_tableContent->setForeChart(reportDataList);
    }
}

//设置范围曲线
void ReportForms::setScopeChart(bool visible,const ReportData& reportData)
{
    m_tableContent->setScopeChartVisible(visible);
    if(visible)
    {
        m_tableContent->setScopeChart(reportData);
    }
}

void ReportForms::printReport()
{
    QPixmap pixmap =  ui->scrollArea->widget()->grab();
    CPrinter printer;
    printer.printPixmap(pixmap);
}

void ReportForms::printPreview()
{
    QPixmap pixmap =  ui->scrollArea->widget()->grab();
    CPrinter printer;
    printer.printPreview(pixmap);
}

void ReportForms::on_prePrint_Btn_clicked()
{
    printPreview();
}

void ReportForms::on_print_Btn_clicked()
{
    printReport();
}
