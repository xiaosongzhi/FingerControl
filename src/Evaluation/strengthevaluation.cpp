#include "strengthevaluation.h"
#include "ui_strengthevaluation.h"
#include "mainwindowpagecontrol.h"
#include "dataFormate.h"
#include "ccommunicateapi.h"
#include <QDebug>
#include <QMapIterator>
#include "currentuserdata.h"
#include "cdatabaseinterface.h"
#include <QDir>
#include <QLayout>
StrengthEvaluation::StrengthEvaluation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StrengthEvaluation),
    m_timer(NULL)
{
    ui->setupUi(this);
    ui->chart_Widget->initChart(LineChart_E,tr("力量曲线"));
    initData();

    m_timer = new QTimer();
    //25Hz
    m_timer->setInterval(40);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotPaintChart()));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));

    colorList << "#888888" << "#7A8FD0" << "#A2D07A" << "#D07AB6" << "#38EBB3";

    ui->fourArea_Widget->setAreaType(1);
    ui->oneArea_Widget->setAreaType(0);
}

StrengthEvaluation::~StrengthEvaluation()
{
    delete ui;
}

void StrengthEvaluation::initData()
{
    QList<int16_t> list;
    for(int i = 0;i < 5;i++)
    {
        dataListMap.insert(i,list);
        QPair<float,float> strength;
        st_FringerStrengthResult.mapFingerStrength.insert(i,strength);
    }
}

void StrengthEvaluation::on_finish_Btn_clicked()
{
    MainWindowPageControl::getInstance()->updatePageRunningflag(StrengthEvaluationPage_E,false);
    //停止绘图
    m_timer->stop();
    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    CCommunicateAPI::getInstance()->getRealTimeStrength(false);
    Sleep(100);
    stopStrengthEvaluation();
    //将数据保存到数据库中
    saveStrengthResult();
    MainWindowPageControl::getInstance()->setCurrentPage(StrengthEvaluationResultPage_E);

    //清除数据
    clearEvluateData();
}

void StrengthEvaluation::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    clearEvluateData();
    //先获取左右手
    int handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->buckling_Widget->setHandType(handType);
    ui->stretch_Widget->setHandType(handType);
    qDebug()<<"开始力量评估";
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));

    statStrengthEvaluation();

    Sleep(100);

    CCommunicateAPI::getInstance()->getRealTimeStrength(true);

    //开启绘图
    Sleep(1000);
    m_timer->start();
}

void StrengthEvaluation::statStrengthEvaluation()
{
    MainWindowPageControl::getInstance()->updatePageRunningflag(StrengthEvaluationPage_E,true);
    uint16_t cmdId;
    cmdId = START_FORCE_ASSESS_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);

    m_startTime = QDateTime::currentDateTime();
}
void StrengthEvaluation::stopStrengthEvaluation()
{
    uint16_t cmdId;
    cmdId = CLOSE_FORCE_ASSESS_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}


void StrengthEvaluation::receiveCommData(QByteArray receiveData)
{
    int length = receiveData[1];
    QByteArray data = receiveData.mid(5,length);
    uint16_t comID;
    memcpy(&comID,receiveData.data()+4,2);
    switch(comID)
    {
    case RESPONDFORCE:
    {
        ST_RealTimeStrength st_RealTimeStrength;
        memcpy(&st_RealTimeStrength,receiveData.data() + 6,sizeof(ST_RealTimeStrength));
        fillChartData(st_RealTimeStrength);
        paintAreaChart(st_RealTimeStrength);
        setStretch(st_RealTimeStrength);
        setBuckling(st_RealTimeStrength);
    }
        break;
    }
}

void StrengthEvaluation::fillChartData(ST_RealTimeStrength &st_RealTimeStrength)
{
    dataListMap[0].append(st_RealTimeStrength.thumb);
    if(dataListMap[0].size() > PAINTNUM)
        dataListMap[0].removeFirst();
    dataListMap[1].append(st_RealTimeStrength.forefinger);
    if(dataListMap[1].size() > PAINTNUM)
        dataListMap[1].removeFirst();
    dataListMap[2].append(st_RealTimeStrength.middlefinger);
    if(dataListMap[2].size() > PAINTNUM)
        dataListMap[2].removeFirst();
    dataListMap[3].append(st_RealTimeStrength.ringfinger);
    if(dataListMap[3].size() > PAINTNUM)
        dataListMap[3].removeFirst();
    dataListMap[4].append(st_RealTimeStrength.littlefinger);
    if(dataListMap[4].size() > PAINTNUM)
        dataListMap[4].removeFirst();
}

void StrengthEvaluation::slotPaintChart()
{
    paintLineChart();
}

//绘制力量域图
void StrengthEvaluation::paintAreaChart(ST_RealTimeStrength& st_RealTimeStrength)
{
    ui->fourArea_Widget->repaintChart(st_RealTimeStrength);
    ui->oneArea_Widget->repaintChart(st_RealTimeStrength.thumb);
}
//绘制力量线图
void StrengthEvaluation::paintLineChart()
{
    QMapIterator<int,QList<int16_t>> iter(dataListMap);
    DataTable dataTable;
    int index = 0;
    while(iter.hasNext())
    {
        iter.next();
        QList<int16_t> list = iter.value();
        DataList dataList;
        QPair<DataList,QColor> dataColorPair;
        for(int i = 0;i < list.size();i++)
        {
            Data tempData;
            tempData.first.setX(i);
            tempData.first.setY(list.at(i)/10.f);
            dataList.append(tempData);
        }
        dataColorPair.first = dataList;
        if(index < colorList.size())
            dataColorPair.second = QColor(colorList.at(index));
        dataTable.append(dataColorPair);
        index++;
    }
    ui->chart_Widget->setChartData(SplineChart_E,dataTable);
    ui->chart_Widget->setAxisValue(Y_Axis,-20,20);
}

//设置伸展力量 负数值
void StrengthEvaluation::setStretch(ST_RealTimeStrength& st_RealTimeStrength)
{
    static int times = 0;
    if(st_RealTimeStrength.thumb <= 0 && (tempStretchStrength.thumb >  st_RealTimeStrength.thumb))
        tempStretchStrength.thumb = st_RealTimeStrength.thumb;
    if(st_RealTimeStrength.forefinger <= 0 && (tempStretchStrength.forefinger >  st_RealTimeStrength.forefinger))
        tempStretchStrength.forefinger = st_RealTimeStrength.forefinger;
    if(st_RealTimeStrength.middlefinger <= 0 && (tempStretchStrength.middlefinger >  st_RealTimeStrength.middlefinger))
        tempStretchStrength.middlefinger = st_RealTimeStrength.middlefinger;
    if(st_RealTimeStrength.ringfinger <= 0 && (tempStretchStrength.ringfinger >  st_RealTimeStrength.ringfinger))
        tempStretchStrength.ringfinger = st_RealTimeStrength.ringfinger;
    if(st_RealTimeStrength.littlefinger <= 0 && (tempStretchStrength.littlefinger >  st_RealTimeStrength.littlefinger))
        tempStretchStrength.littlefinger = st_RealTimeStrength.littlefinger;

    //设置手指颜色
    if(times > 5)
    {
        ui->stretch_Widget->setFingerColorByStrength(tempStretchStrength);
        times = 0;
    }
    times++;
    ui->thumbS_Label->setText(QString::number(tempStretchStrength.thumb/-10.f,'f',1));
    ui->forefingerS_Label->setText(QString::number(tempStretchStrength.forefinger/-10.f,'f',1));
    ui->middleFingerS_Label->setText(QString::number(tempStretchStrength.middlefinger/-10.f,'f',1));
    ui->ringFingerS_Label->setText(QString::number(tempStretchStrength.ringfinger/-10.f,'f',1));
    ui->littleFingerS_Label->setText(QString::number(tempStretchStrength.littlefinger/-10.f,'f',1));

    int tension = (tempStretchStrength.thumb/-10.f) + (tempStretchStrength.forefinger/-10.f)
            +(tempStretchStrength.middlefinger/-10.f) + (tempStretchStrength.ringfinger/-10.f)
            +(tempStretchStrength.littlefinger/-10.f);
    ui->tension_Label->setText(QString::number(tension/(5.f)*0.8,'f',1));

    st_FringerStrengthResult.mapFingerStrength[0].second = tempStretchStrength.thumb/-10.f;
    st_FringerStrengthResult.mapFingerStrength[1].second = tempStretchStrength.forefinger/-10.f;
    st_FringerStrengthResult.mapFingerStrength[2].second = tempStretchStrength.middlefinger/-10.f;
    st_FringerStrengthResult.mapFingerStrength[3].second = tempStretchStrength.ringfinger/-10.f;
    st_FringerStrengthResult.mapFingerStrength[4].second = tempStretchStrength.littlefinger/-10.f;
}
//设置屈曲力 正数值
void StrengthEvaluation::setBuckling(ST_RealTimeStrength& st_RealTimeStrength)
{
    static int buckTimes = 0;
    if(st_RealTimeStrength.thumb >= 0 && tempBucklingStrength.thumb <  st_RealTimeStrength.thumb)
        tempBucklingStrength.thumb = st_RealTimeStrength.thumb;
    if(st_RealTimeStrength.forefinger >= 0 && tempBucklingStrength.forefinger <  st_RealTimeStrength.forefinger)
        tempBucklingStrength.forefinger = st_RealTimeStrength.forefinger;
    if(st_RealTimeStrength.middlefinger >= 0 && tempBucklingStrength.middlefinger <  st_RealTimeStrength.middlefinger)
        tempBucklingStrength.middlefinger = st_RealTimeStrength.middlefinger;
    if(st_RealTimeStrength.ringfinger >= 0 && tempBucklingStrength.ringfinger <  st_RealTimeStrength.ringfinger)
        tempBucklingStrength.ringfinger = st_RealTimeStrength.ringfinger;
    if(st_RealTimeStrength.littlefinger >= 0 && tempBucklingStrength.littlefinger <  st_RealTimeStrength.littlefinger)
        tempBucklingStrength.littlefinger = st_RealTimeStrength.littlefinger;
    ui->thumbQ_Label->setText(QString::number(tempBucklingStrength.thumb/10.f,'f',1));
    ui->forefingerQ_Label->setText(QString::number(tempBucklingStrength.forefinger/10.f,'f',1));
    ui->middleFingerQ_Label->setText(QString::number(tempBucklingStrength.middlefinger/10.f,'f',1));
    ui->ringFingerQ_Label->setText(QString::number(tempBucklingStrength.ringfinger/10.f,'f',1));
    ui->littleFingerQ_Label->setText(QString::number(tempBucklingStrength.littlefinger/10.f,'f',1));

    //设置手指颜色
    if(buckTimes > 5)
    {
        ui->buckling_Widget->setFingerColorByStrength(tempBucklingStrength);
        buckTimes = 0;
    }
    buckTimes++;

    int grip = (tempBucklingStrength.thumb/-10.f) + (tempBucklingStrength.forefinger/-10.f)
            +(tempBucklingStrength.middlefinger/-10.f) + (tempBucklingStrength.ringfinger/-10.f)
            +(tempBucklingStrength.littlefinger/-10.f);
    ui->grip_Label->setText(QString::number(grip/(-5.f)*0.8,'f',1));

    st_FringerStrengthResult.mapFingerStrength[0].first = tempBucklingStrength.thumb/10.f;
    st_FringerStrengthResult.mapFingerStrength[1].first = tempBucklingStrength.forefinger/10.f;
    st_FringerStrengthResult.mapFingerStrength[2].first = tempBucklingStrength.middlefinger/10.f;
    st_FringerStrengthResult.mapFingerStrength[3].first = tempBucklingStrength.ringfinger/10.f;
    st_FringerStrengthResult.mapFingerStrength[4].first = tempBucklingStrength.littlefinger/10.f;
}

void StrengthEvaluation::saveStrengthResult()
{

    st_FringerStrengthResult.ID = CurrentUserData::getInstace()->getCurrentUserID();
    st_FringerStrengthResult.hand = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    st_FringerStrengthResult.dateTime = m_startTime;
    st_FringerStrengthResult.grip = ui->grip_Label->text().toFloat();
    st_FringerStrengthResult.tension = ui->tension_Label->text().toFloat();

    //存储评估结果
    QString table("strengthresulttable");
    if(CDatabaseInterface::getInstance()->insertRowTable(table,fringerStrengthResultToVariantMap(st_FringerStrengthResult)))
        qDebug()<<"力量评估存储成功";
    else
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();

    st_trainReport.trainMode = TRAINMODE_STRENGTH_E;
    st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
    st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;
    st_trainReport.startTime = m_startTime;
    st_trainReport.endTime = QDateTime::currentDateTime();
    st_trainReport.duration = m_startTime.secsTo(st_trainReport.endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.resultImagePath = getImageName();
    st_trainReport.markMsg = QString(tr("屈曲力%1N,伸展力%2N")).arg(ui->grip_Label->text().toFloat()).arg(ui->tension_Label->text().toFloat());
    //将训练数据存入数据库
    QString table1("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table1,trainReportToVariantMap(st_trainReport)))
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
    }
    else
        qDebug()<<"力量评估训练报告存储成功";
    //存储图片
    QPixmap buckPix = ui->buckling_Widget->grab();
    QPixmap stretPix = ui->stretch_Widget->grab();

    QString dirpath(QApplication::applicationDirPath() + "/strengthResult/");
    QDir resultDir(dirpath);
    if(!resultDir.exists())
    {
        resultDir.mkdir(dirpath);
    }
    buckPix.save(dirpath + "/buckPix.png");
    stretPix.save(dirpath +"/stretPix.png");
}

QString StrengthEvaluation::getImageName()
{
    //图片命名方法 ID+时间+描述 例：10010_20210910123456_force.png
    QString dirPath = QApplication::applicationDirPath() + "/result";
    QDir resultDir(dirPath);
    if(!resultDir.exists())
        resultDir.mkdir(dirPath);

    QString fileName = QString::number(st_trainReport.ID) + "_" +
            m_startTime.toString("yyyyMMddhhmmss") +"_force.png";
    QString absulatePath = dirPath + "/"  + fileName;
    return absulatePath;
}

//关闭当前界面的训练
void StrengthEvaluation::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == StrengthEvaluationPage_E)
    {
        on_finish_Btn_clicked();
    }
}

void StrengthEvaluation::clearEvluateData()
{
    //清除右侧显示label
    tempStretchStrength.forefinger = 0;
    tempStretchStrength.littlefinger = 0;
    tempStretchStrength.middlefinger = 0;
    tempStretchStrength.ringfinger = 0;
    tempStretchStrength.thumb = 0;

    tempBucklingStrength.forefinger = 0;
    tempBucklingStrength.littlefinger = 0;
    tempBucklingStrength.middlefinger = 0;
    tempBucklingStrength.ringfinger = 0;
    tempBucklingStrength.thumb = 0;

    setStretch(tempStretchStrength);
    setBuckling(tempBucklingStrength);

    ui->grip_Label->setText("0");
    ui->tension_Label->setText("0");
    //重置域图
    ui->fourArea_Widget->resetAreatChart();
    ui->oneArea_Widget->resetAreatChart();

    //清除手指显示结果
    ui->buckling_Widget->clearFingerColorEffect();
    ui->stretch_Widget->clearFingerColorEffect();
}
