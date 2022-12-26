#include "motionscopeevaluationresult.h"
#include "ui_motionscopeevaluationresult.h"
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include <QDebug>
#include <QApplication>
#include <QDir>
#include "logger.h"
#include <QMessageBox>
//LOG4QT_DECLARE_STATIC_LOGGER(logger, MotionScopeEvaluationResult)
MotionScopeEvaluationResult::MotionScopeEvaluationResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotionScopeEvaluationResult)
{
    ui->setupUi(this);
    ui->lineChart_Widget->initChart(LineChart_E,tr("活动度结果"));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
}

MotionScopeEvaluationResult::~MotionScopeEvaluationResult()
{
    delete ui;
}

void MotionScopeEvaluationResult::on_finish_Btn_clicked()
{
    if(saveImageResult())
        QMessageBox::warning(NULL,tr("提示"),tr("当前结果保存成功"));
    else
        QMessageBox::warning(NULL,tr("提示"),tr("当前结果保存失败"));
    this->close();
    MainWindowPageControl::getInstance()->setCurrentPage(EvaluationPage_E);
}

void MotionScopeEvaluationResult::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    displayResult();
}

void MotionScopeEvaluationResult::displayResult()
{
    QString tableName("jointrangeresulttable");
    int id = CurrentUserData::getInstace()->getCurrentUserID();
    QString query(QString("select * from %1 where ID ='%2' order by %3 DESC").arg(tableName).arg(id).arg("startTime"));
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,count);
    if(count > 0)
    {
        //最新一次
        ST_JointRangeResult st_JointRangeResult = variantMapToJointRangeResult(listMap.at(0));

        m_st_jointRangeResult = st_JointRangeResult;
        if(0 == st_JointRangeResult.hand)
            ui->hand_Label->setText(tr("左手"));
        else if(1 == st_JointRangeResult.hand)
            ui->hand_Label->setText(tr("右手"));
        ui->totoalPercent_Label->setText(QString::number(st_JointRangeResult.totalPercent));
        ui->date_Label->setText(st_JointRangeResult.startDateTime.toString("MM/dd"));
        ui->time_Label->setText(st_JointRangeResult.startDateTime.toString("hh:mm"));
        int durationTime = st_JointRangeResult.startDateTime.secsTo(st_JointRangeResult.endDateTime);
        if(durationTime < 60)
            durationTime = 60;
        ui->durationTime_Label->setText(QString::number(durationTime/60));

        if(st_JointRangeResult.mapMovePercent.size() >= 5)
        {
            ui->thumb_Label->setText(QString::number(st_JointRangeResult.mapMovePercent.take(0)));
            ui->forefinger_Label->setText(QString::number(st_JointRangeResult.mapMovePercent.take(1)));
            ui->middleFinger_Label->setText(QString::number(st_JointRangeResult.mapMovePercent.take(2)));
            ui->ringFinger_Label->setText(QString::number(st_JointRangeResult.mapMovePercent.take(3)));
            ui->littleFinger_Label->setText(QString::number(st_JointRangeResult.mapMovePercent.take(4)));
        }
        DataTable dataTable;
        QList<Data> dataList;
        for(int i = 0;i < count;i++)
        {
            Data tempData;
            ST_JointRangeResult st_tempJointRangeResult;
            tempData.first.setX(i);
            tempData.first.setY(variantMapToJointRangeResult(listMap.at(i)).totalPercent);
            dataList.append(tempData);
        }
        QPair<DataList,QColor> dataColorPair;
        dataColorPair.first = dataList;
        dataTable.append(dataColorPair);
        ui->lineChart_Widget->setChartData(LineChart_E,dataTable);
        ui->lineChart_Widget->setAxisValue(Y_Axis,0,100);
    }
    else   
    {
        qDebug()<<"未查询到关节活动度结果数据";
//        logger()->debug("未查询到关节活动度结果数据");
    }
}

//保存图片
bool MotionScopeEvaluationResult::saveImageResult()
{
    //图片命名方法 ID+时间+描述 例：10010_20210910123456_关节活动度.png
    QString dirPath = QApplication::applicationDirPath() + "/result";
    QDir resultDir(dirPath);
    if(!resultDir.exists())
        resultDir.mkdir(dirPath);

    QString fileName = QString::number(m_st_jointRangeResult.ID) + "_" +
            m_st_jointRangeResult.startDateTime.toString("yyyyMMddhhmm") +"_scope.png";
    QString absulatePath = dirPath + "/"  + fileName;
    QPixmap pixmap = ui->grab_GroupBox->grab();
    if(!pixmap.save(absulatePath))
    {
        qDebug()<<"图片保存失败";
//        logger()->debug("结果图片保存失败");
        return false;
    }
    return true;

}

void MotionScopeEvaluationResult::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == MotionScopeEvaluationResultPage_E)
    {
        on_finish_Btn_clicked();
    }
}
