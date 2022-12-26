#include "strengthevaluationresult.h"
#include "ui_strengthevaluationresult.h"
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
StrengthEvaluationResult::StrengthEvaluationResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StrengthEvaluationResult)
{
    ui->setupUi(this);
    ui->lineChart_Widget->initChart(LineChart_E,tr("力量走势"));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
}

StrengthEvaluationResult::~StrengthEvaluationResult()
{
    delete ui;
}

void StrengthEvaluationResult::on_finish_Btn_clicked()
{
    if(saveImageResult())
        QMessageBox::warning(NULL,tr("提示"),tr("当前结果保存成功"));
    else
        QMessageBox::warning(NULL,tr("提示"),tr("当前结果保存失败"));
    this->close();
    MainWindowPageControl::getInstance()->setCurrentPage(EvaluationPage_E);
}

void StrengthEvaluationResult::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    displayResultData();
}

void StrengthEvaluationResult::displayResultData()
{
    QString tableName("strengthresulttable");
    int id = CurrentUserData::getInstace()->getCurrentUserID();
    QString query(QString("select * from %1 where ID ='%2' order by %3 DESC").arg(tableName).arg(id).arg("dateTime"));
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,count);
    if(count > 0)
    {
        ST_FringerStrengthResult st_FringerStrength = variantMapToFringerStrengthResult(listMap.at(0));
        m_st_fingerStrengthResult = st_FringerStrength;
        if(0 == st_FringerStrength.hand)
            ui->hand_Label->setText(tr("左手"));
        else if(1 == st_FringerStrength.hand)
            ui->hand_Label->setText(tr("右手"));
        ui->ID_Label->setText(QString::number(st_FringerStrength.ID));
        ui->grip_Label->setText(QString::number(st_FringerStrength.grip));
        ui->tension_Label->setText(QString::number(st_FringerStrength.tension));

        ui->thumbQ_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[0].first));
        ui->thumbS_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[0].second*(1)));

        ui->forefingerQ_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[1].first));
        ui->forefingerS_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[1].second*(1)));

        ui->middleFingerQ_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[2].first));
        ui->middleFingerS_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[2].second*(1)));

        ui->ringFingerQ_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[3].first));
        ui->ringFingerS_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[3].second*(1)));

        ui->littleFingerQ_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[4].first));
        ui->littleFingerS_Label->setText(QString::number(st_FringerStrength.mapFingerStrength[4].second*(1)));

        QString date = QDateTime::currentDateTime().toString("MM/dd");
        ui->date_Label->setText(date);
        QString time = QDateTime::currentDateTime().toString("hh:mm");
        ui->time_Label->setText(time);

        DataTable dataTable;
        QPair<DataList,QColor> dataColorPairQ;
        QPair<DataList,QColor> dataColorPairS;
        QList<Data> DataListQ,DataListS;
        for(int i = 0;i <count;i++)
        {
            Data tempDataQ,tempDataS;
            ST_FringerStrengthResult st_tempFringerStrength = variantMapToFringerStrengthResult(listMap.at(i));
            tempDataQ.first.setX(i);
            tempDataQ.first.setY(st_tempFringerStrength.grip);
            tempDataQ.second = st_tempFringerStrength.dateTime.toString("yyyy/MM/dd hh:mm");
            tempDataS.first.setX(i);
            tempDataS.first.setY(st_tempFringerStrength.tension);
            tempDataS.second = st_tempFringerStrength.dateTime.toString("yyyy/MM/dd hh:mm");
            DataListQ.append(tempDataQ);
            DataListS.append(tempDataS);
        }
        dataColorPairQ.first = DataListQ;
        dataColorPairQ.second = QColor("#70E086");
        dataColorPairS.first = DataListS;
        dataColorPairS.second = QColor("#7A8FD0");
        dataTable.append(dataColorPairQ);
        dataTable.append(dataColorPairS);
        ui->lineChart_Widget->setChartData(LineChart_E,dataTable);

        QString dirpath(QApplication::applicationDirPath() + "/strengthResult/");

        QPixmap tensionPix(dirpath + "/stretPix.png");
        ui->tensionResult_Label->setPixmap(tensionPix);
        QPixmap gripPix(dirpath + "/buckPix.png");
        ui->gripResult_Label->setPixmap(gripPix);
    }
}

//保存图片
bool StrengthEvaluationResult::saveImageResult()
{
    //图片命名方法 ID+时间+描述 例：10010_20210910123456_力量评定.png

    QString dirPath = QApplication::applicationDirPath() + "/result";
    QDir resultDir(dirPath);
    if(!resultDir.exists())
        resultDir.mkdir(dirPath);

    QString fileName = QString::number(m_st_fingerStrengthResult.ID) + "_" +
            m_st_fingerStrengthResult.dateTime.toString("yyyyMMddhhmmss") +"_force.png";
    QString absulatePath = dirPath + "/"  + fileName;
    QPixmap pixmap = ui->grab_GroupBox->grab();
    if(!pixmap.save(absulatePath))
    {
        qDebug()<<"力量评定图片保存失败";
        return false;
    }
    return true;
}

void StrengthEvaluationResult::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == StrengthEvaluationResultPage_E)
    {
        qDebug()<<"关闭力量评估结果界面";
        on_finish_Btn_clicked();
    }
}
