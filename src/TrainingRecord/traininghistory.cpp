#include "traininghistory.h"
#include "ui_traininghistory.h"
#include "cdatabaseinterface.h"
#include "currentuserdata.h"
#include <QScrollBar>
#include <QDebug>
#include <QMessageBox>
TrainingHistory::TrainingHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingHistory),
    m_currentPage(1),
    m_perPageCount(5),
    m_maxRow(100),
    m_totalPage(0),
    m_totalReportNum(0),
    m_reportDialog(NULL)
{
    ui->setupUi(this);
    initTableWidget();
    QObjectList list = ui->buttons_GroupBox->children();
    foreach(QObject* object,list)
    {
        if(object->objectName().contains("view"))
            connect(object,SIGNAL(clicked()),this,SLOT(slotViewBtn()));
        else if(object->objectName().contains("import"))
            connect(object,SIGNAL(clicked()),this,SLOT(slotImportBtn()));
    }

    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));

    m_reportDialog = new ReportForms();

    ui->tips_GroupBox->setVisible(false);
}

TrainingHistory::~TrainingHistory()
{
    if(m_reportDialog)
    {
        m_reportDialog->deleteLater();
        m_reportDialog = NULL;
    }
    delete ui;
}

void TrainingHistory::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    flushTableData();
}

//刷新表格
void TrainingHistory::flushTableData()
{
    int currentID = CurrentUserData::getInstace()->getCurrentPatientMsg().ID;

    QString orderWord("startTime");
    QString query(QString("select * from traintable where ID = '%1' order by %2 DESC").arg(currentID).arg(orderWord));
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();

    //隐藏按钮
    setRowButtonHide(-1,true);
    //无数据
    if(-1 == count)
    {
        ui->page_Label->setText("1/1");
        //清除表格
        ui->tableWidget->clearContents();
        return;
    }

    m_totalPage = count/m_perPageCount + 1;
    m_currentPage = 1;
    //设置当前页数
    ui->page_Label->setText(QString("1/%2").arg(m_totalPage));
    m_historyListMap = CDatabaseInterface::getInstance()->getValues(0,count < m_maxRow ? count:m_maxRow);
    m_totalReportNum = m_historyListMap.size();
    if(!m_historyListMap.isEmpty())
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i <m_historyListMap.size();i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::on_previous_Btn_clicked()
{
    if(m_currentPage <= 1)
        return;
    m_currentPage--;
    //设置当前页数
    ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
    //当前首个数据条下标
    int firstIndex = (m_currentPage-1)*m_perPageCount;
    //剩余的个数
    int surplus = m_totalReportNum - firstIndex;
    //本页要添加的个数
    int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;

    setRowButtonHide(-1,true);
    if(toAddNum > 0)
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i <toAddNum;i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i+firstIndex));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::on_next_Btn_clicked()
{
    if(m_currentPage < m_totalPage)
        m_currentPage++;
    else
        return;
    //设置当前页数
    ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
    //当前首个数据条下标
    int firstIndex = (m_currentPage-1)*m_perPageCount;
    //剩余的个数
    int surplus = m_totalReportNum - firstIndex;
    //本页要添加的个数
    int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;
    //还有下一页数据
    setRowButtonHide(-1,true);
    if(toAddNum > 0)
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i < toAddNum;i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i+firstIndex));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::initTableWidget()
{
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(5);
    for(int i = 0;i <ui->tableWidget->columnCount();i++)
        ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()/ui->tableWidget->columnCount()-1);
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
        ui->tableWidget->setRowHeight(i,ui->tableWidget->height()/ui->tableWidget->rowCount()-1);
    ui->tableWidget->horizontalScrollBar()->hide();
    ui->tableWidget->verticalScrollBar()->hide();
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setEnabled(false);
}

void TrainingHistory::slotViewBtn()
{
    //"view0_Btn"
    QString buttonName = sender()->objectName();
    int index = buttonName.indexOf('_');
    int indexText = buttonName.mid(index-1,1).toInt();

    QTableWidgetItem *dateItem = ui->tableWidget->item(indexText,0);
    QTableWidgetItem *timeItem = ui->tableWidget->item(indexText,1);

    QString dateTimeText = dateItem->text()+"T"+timeItem->text();
    QDateTime dateTime = QDateTime::fromString(dateTimeText,"yyyy-MM-dd hh:mm:ss.zzz");

    QString query(QString("select * from traintable where startTime = '%1'").arg(dateTimeText));
    //查询当前信息并生成报告
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(-1 == count)
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
        QMessageBox::warning(NULL,tr("提示"),tr("未查询到结果！"));
        return;
    }

    QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,1);
    if(!vMapList.isEmpty())
    {
        QVariantMap vMap = vMapList.at(0);
        ST_TrainReport st_TrainReport = variantMapToTrainReport(vMap);
        //回看报告
        if(!st_TrainReport.resultImagePath.isEmpty())
        {
            QPixmap pix(st_TrainReport.resultImagePath);
            resultLabel.setPixmap(pix);
            resultLabel.resize(1440,840);
            resultLabel.show();
        }
    }
}

void TrainingHistory::slotImportBtn()
{
    //"import1_Btn"
    QString buttonName = sender()->objectName();
    int index = buttonName.indexOf('_');
    int indexText = buttonName.mid(index-1,1).toInt();

    QTableWidgetItem *dateItem = ui->tableWidget->item(indexText,0);
    QTableWidgetItem *timeItem = ui->tableWidget->item(indexText,1);

    QString dateTime = dateItem->text() +timeItem->text();
}

//在表中填充数据
void TrainingHistory::fillTableWidget(ST_TrainReport& st_TrainReport,int row)
{
    //date
    QTableWidgetItem *dateItem = new QTableWidgetItem();
    dateItem->setTextAlignment(Qt::AlignCenter);

    dateItem->setText(st_TrainReport.startTime.toString("yyyy-MM-dd"));
    ui->tableWidget->setItem(row,0,dateItem);
    //    //time
    QTableWidgetItem *timeItem = new QTableWidgetItem();
    timeItem->setTextAlignment(Qt::AlignCenter);

    timeItem->setText(st_TrainReport.startTime.toString("hh:mm:ss.zzz"));
    ui->tableWidget->setItem(row,1,timeItem);
    //    //时长
    QTableWidgetItem *durationItem = new QTableWidgetItem();
    durationItem->setTextAlignment(Qt::AlignCenter);
    durationItem->setText(QString::number(st_TrainReport.duration));
    ui->tableWidget->setItem(row,2,durationItem);
    //    //运动模式
    QTableWidgetItem *modeItem = new QTableWidgetItem();
    modeItem->setTextAlignment(Qt::AlignCenter);
    switch(st_TrainReport.trainMode)
    {
    case TRAINMODE_PASSIVE_E:
        modeItem->setText(tr("被动模式"));
        break;
    case TRAINMODE_ACTIVE_E:
        modeItem->setText(tr("主动模式"));
        break;
    case TRAINMODE_BOOST_E:
        modeItem->setText(tr("助力模式"));
        break;
    case TRAINMODE_MEMORY_E:
        modeItem->setText(tr("记忆模式"));
        break;
    case TRAINMODE_STRENGTH_E:
        modeItem->setText(tr("力量评定模式"));
        break;
    case TRAINMODE_SCOPE_E:
        modeItem->setText(tr("范围评估模式"));
        break;
    case TRAINMODE_GAME_E:
        modeItem->setText(tr("游戏训练"));
        break;
    }
    ui->tableWidget->setItem(row,3,modeItem);
    //总结
    QTableWidgetItem *scoreItem = new QTableWidgetItem();
    scoreItem->setTextAlignment(Qt::AlignCenter);
    scoreItem->setText(st_TrainReport.markMsg);
    ui->tableWidget->setItem(row,4,scoreItem);
}

void TrainingHistory::setRowButtonHide(int row,bool hide)
{
    QObjectList list = ui->buttons_GroupBox->children();

    if(-1 == row)
    {
        foreach(QObject* object,list)
        {
            if(object->objectName().contains("view") ||object->objectName().contains("import") )
            {
                QPushButton* button = dynamic_cast<QPushButton*>(object);
                button->setVisible(!hide);
            }

        }
    }
    else
    {
        foreach(QObject* object,list)
        {
            if(object->objectName().contains(QString("view%1").arg(row)) || object->objectName().contains(QString("import%1").arg(row)))
            {
                QPushButton* button = dynamic_cast<QPushButton*>(object);
                button->setVisible(!hide);
            }
        }
    }
}

void TrainingHistory::slotCurrentUserChanged()
{
    flushTableData();
}

void TrainingHistory::on_printReport_Btn_clicked()
{
    ui->tips_GroupBox->setVisible(true);
}

void TrainingHistory::on_tipConfirm_Btn_clicked()
{
    ui->tips_GroupBox->setVisible(false);
    //填充表头信息
    m_reportDialog->clearReportTitle();
    ST_PatientMsg st_patientMsg;
    st_patientMsg = CurrentUserData::getInstace()->getCurrentPatientMsg();
    m_reportDialog->setReportTitleMessage(st_patientMsg);

    bool isChecked = ui->force_RadioButton->isChecked();
    //查询力量结果数据库表
    ReportDataList reportDataList;
    if(getForceEvaluationData(reportDataList) > 0)
        m_reportDialog->setForeChart(isChecked,reportDataList);
    else
        qDebug()<<"无力量评估数据";
    //训练时间(主要获取被动、主动、助力训练的时间)
    int data[3] = {0};
    if(calculateTrainDurationTime(data) > 0)
    {
        isChecked = ui->time_RadioButton->isChecked();
        m_reportDialog->setHistoryTimeChart(isChecked,data,3);
    }
    else
        qDebug()<<"无训练时间统计数据";
    //关节活动度范围评估
    ReportData reportData;
    isChecked = ui->scope_RadioButton->isChecked();
    if(getScopeEvaluationData(reportData) > 0)
        m_reportDialog->setScopeChart(isChecked,reportData);
    else
        qDebug()<<"无范围评估数据";

    m_reportDialog->show();
}

//计算各种训练的时长
int TrainingHistory::calculateTrainDurationTime(int* data)
{
    //1.根据当前ID将训练的所有数据采集出来并保存到QMap<mode,duration>中
    int ID = CurrentUserData::getInstace()->getCurrentUserID();

    QString query(QString("select * from traintable where ID = '%1' order by startTime DESC").arg(ID));
    if(!CDatabaseInterface::getInstance()->exec(query))
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();

    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count > 0)
    {
        QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,count);

        int passiveTime = 0,activeTime = 0,boostTime = 0;
        for(int i = 0;i < vMapList.size();i++)
        {
            QVariantMap vMap = vMapList.at(i);
            ST_TrainReport st_TrainReport = variantMapToTrainReport(vMap);
            //按照被动、主动、助力顺序赋值
            switch((E_TRAINMODE)st_TrainReport.trainMode)
            {
            case TRAINMODE_PASSIVE_E:
                passiveTime += st_TrainReport.duration;
                break;
            case TRAINMODE_ACTIVE_E:
                activeTime += st_TrainReport.duration;
                break;
            case TRAINMODE_BOOST_E:
                boostTime += st_TrainReport.duration;
                break;
            }
        }
        data[0] = passiveTime;
        data[1] = activeTime;
        data[2] = boostTime;
    }
    return count;
}

//获取力量评估历史数据
int TrainingHistory::getForceEvaluationData(ReportDataList &reportDataList)
{
    //这样使用数据 当训练总数<=5时显示所有，当大于5时，则显示最近5个
    int ID = CurrentUserData::getInstace()->getCurrentUserID();

    QString query(QString("select * from strengthresulttable where ID = '%1' order by dateTime DESC").arg(ID));
    if(!CDatabaseInterface::getInstance()->exec(query))
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();

    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count > 0)
    {
        QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,count<5? count:5);
        ReportData reportGripData;   //握力
        ReportData reportTensionData;//伸展力
        for(int i = 0; i< vMapList.size();i++)
        {
            ST_FringerStrengthResult st_fringerStrengthResult = variantMapToFringerStrengthResult(vMapList.at(i));
            reportGripData.insert(st_fringerStrengthResult.dateTime,st_fringerStrengthResult.grip);
            reportTensionData.insert(st_fringerStrengthResult.dateTime,st_fringerStrengthResult.tension);
        }
        reportDataList.append(reportGripData);
        reportDataList.append(reportTensionData);
    }
    return count;
}

//获取范围评估历史数据
int TrainingHistory::getScopeEvaluationData(ReportData &reportData)
{
    //这样使用数据 当训练总数<=5时显示所有，当大于5时，则显示最近5个
    int ID = CurrentUserData::getInstace()->getCurrentUserID();

    QString query(QString("select * from jointrangeresulttable where ID = '%1' order by startTime DESC").arg(ID));
    if(!CDatabaseInterface::getInstance()->exec(query))
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();

    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count > 0)
    {
        QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,count<5? count:5);
        for(int i = 0; i< vMapList.size();i++)
        {
            ST_JointRangeResult st_jointRangeResult = variantMapToJointRangeResult(vMapList.at(i));
            reportData.insert(st_jointRangeResult.startDateTime,st_jointRangeResult.totalPercent);
        }
    }
    return count;
}
