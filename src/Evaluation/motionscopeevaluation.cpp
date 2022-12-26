#include "motionscopeevaluation.h"
#include "ui_motionscopeevaluation.h"
#include "mainwindowpagecontrol.h"
#include "dataFormate.h"
#include "ccommunicateapi.h"
#include "currentuserdata.h"
#include <QDebug>
#include <QMapIterator>
#include "cdatabaseinterface.h"
#include <QDir>
#include "logger.h"
//LOG4QT_DECLARE_STATIC_LOGGER(logger, CUdpInterface)
MotionScopeEvaluation::MotionScopeEvaluation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotionScopeEvaluation),
    m_evaluateType(1)
{
    ui->setupUi(this);
    connect(ui->train_Widget,&CTrainingHand::signalFingerStateChanged,this,&MotionScopeEvaluation::slotFingerStateChanged);
    connect(MainWindowPageControl::getInstance(),&MainWindowPageControl::signalSwitchPage,this,&MotionScopeEvaluation::slotCurrentPageChanged);
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));
    initWidget();
    for(int i = 0;i < 5;i++)
        fingersState[i] = 1;

    ui->train_Widget->switchScope(true);


}

MotionScopeEvaluation::~MotionScopeEvaluation()
{
    delete ui;
}

void MotionScopeEvaluation::initWidget()
{
    boostFourNormal = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostFourNoSelected.png);}\n"
                      "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/boostFourHover.png);}";

    boostFourSelected = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostFourSelected.png);}";

    boostOneNormal = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostOneNoSelected.png);}\n"
                     "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/boostOneHover.png);}";
    boostOneSelected = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostOneSelected.png);}";

    ui->fourFinger_Btn->setStyleSheet(boostFourSelected);
}

void MotionScopeEvaluation::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    //获取当前用户的手信息
    st_HandData = CurrentUserData::getInstace()->getCurrentUserHandData();
    //设置左右手
    ui->train_Widget->switchHand(st_HandData.hand);
    //设置相应的手指状态（使能与否）
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    //获取实时位置
    startMotionRangeEvaluate();
    Sleep(100);

    CCommunicateAPI::getInstance()->getRealTimePositon(true);

    m_StartDateTime = QDateTime::currentDateTime();

    for(int i =0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
    }

    //初始化手指状态
    initHandFingers();

    //设置四指评定
    on_fourFinger_Btn_clicked();
}

void MotionScopeEvaluation::initHandFingers()
{
    for(int i =0;i < 5;i++)
    {
        if(fingersState[i])
            ui->train_Widget->setHandFingerEnable(st_HandData.hand,i,true);
        else
            ui->train_Widget->setHandFingerEnable(st_HandData.hand,i,false);
    }
}

void MotionScopeEvaluation::on_fourFinger_Btn_clicked()
{
    if(ui->fourFinger_Btn->styleSheet() == boostFourNormal)
    {
        ui->fourFinger_Btn->setStyleSheet(boostFourSelected);
        ui->oneFinger_Btn->setStyleSheet(boostOneNormal);
    }
    m_evaluateType = 1;
    for(int i = 1;i < 5;i++)
        ui->train_Widget->setHandFingerEnable(st_HandData.hand,i,true);

    ui->train_Widget->setHandFingerEnable(st_HandData.hand,0,false);

    uint16_t cmdId = MOTION_RANG_JUDGE_FINGER_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(uint8_t));
    array[0] = 2;
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MotionScopeEvaluation::on_oneFinger_Btn_clicked()
{
    if(ui->oneFinger_Btn->styleSheet() == boostOneNormal)
    {
        ui->oneFinger_Btn->setStyleSheet(boostOneSelected);
        ui->fourFinger_Btn->setStyleSheet(boostFourNormal);
    }
    m_evaluateType = 2;
    for(int i = 1;i < 5;i++)
        ui->train_Widget->setHandFingerEnable(st_HandData.hand,i,false);

    ui->train_Widget->setHandFingerEnable(st_HandData.hand,0,true);
    uint16_t cmdId = MOTION_RANG_JUDGE_FINGER_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(uint8_t));
    array[0] = 1;
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MotionScopeEvaluation::on_stretch_Dial_valueChanged(int value)
{
    int16_t strength = 0;
    strength = value;
    uint16_t cmdId = MOTION_RANG_JUDGE_EXTEND_FORCE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int16_t));
    memcpy(array.data(),&strength,sizeof(int16_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MotionScopeEvaluation::on_buckking_Dial_valueChanged(int value)
{
    int16_t strength = 0;
    strength = value;
    uint16_t cmdId = MOTION_RANG_JUDGE_FLEXION_FORCE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int16_t));
    memcpy(array.data(),&strength,sizeof(int16_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//完成评估
void MotionScopeEvaluation::on_finish_Btn_clicked()
{
    MainWindowPageControl::getInstance()->updatePageRunningflag(MotionScopeEvaluationPage_E,false);
    CCommunicateAPI::getInstance()->getRealTimePositon(false);
    Sleep(100);
    stopMotionRangeEvaluate();
    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    saveMotionScopeResult();
    MainWindowPageControl::getInstance()->setCurrentPage(MotionScopeEvaluationResultPage_E);
    clearSetAndParam();
}

void MotionScopeEvaluation::startMotionRangeEvaluate()
{
    MainWindowPageControl::getInstance()->updatePageRunningflag(MotionScopeEvaluationPage_E,true);
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    CCommunicateAPI::getInstance()->getRealTimePositon(true);

    uint16_t cmdId = START_MOTION_RANG_JUDGE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//结束关节活动度评估
void MotionScopeEvaluation::stopMotionRangeEvaluate()
{
    uint16_t cmdId = CLOSE_MOTION_RANG_JUDGE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MotionScopeEvaluation::receiveCommData(QByteArray receiveData)
{
    int length = receiveData[1];
    QByteArray data = receiveData.mid(5,length);
    uint16_t comID;
    memcpy(&comID,receiveData.data()+4,2);

    switch(comID)
    {
    case RESPONDPOSITION: //获取手指实时位置数据
    {
        ST_RealTimeFingersPosition st_RealTimePosition;
        memcpy(&st_RealTimePosition,receiveData.data() + 6,sizeof(ST_RealTimeFingersPosition));
        setHandFingersPosition(st_RealTimePosition);
    }
        break;
    }
}

void MotionScopeEvaluation::setHandFingersPosition(ST_RealTimeFingersPosition st_FingerPosition)
{
    //拇指评定
    if(2 == m_evaluateType)
    {
        if(0 == st_HandData.hand)
        {
            if(fingersState[0])
                ui->train_Widget->setHandFingerValue(st_HandData.hand,0,100 - st_FingerPosition.Lthumb);
        }
        else if(1 == st_HandData.hand)
        {
            if(fingersState[0])
                ui->train_Widget->setHandFingerValue(st_HandData.hand,0,st_FingerPosition.Rthumb);
        }
    }//四指评定
    else if(1 == m_evaluateType)
    {
        if(fingersState[1])
            ui->train_Widget->setHandFingerValue(st_HandData.hand,1,st_FingerPosition.forefinger);
        if(fingersState[2])
            ui->train_Widget->setHandFingerValue(st_HandData.hand,2,st_FingerPosition.middlefinger);
        if(fingersState[3])
            ui->train_Widget->setHandFingerValue(st_HandData.hand,3,st_FingerPosition.ringfinger);
        if(fingersState[4])
            ui->train_Widget->setHandFingerValue(st_HandData.hand,4,st_FingerPosition.littlefinger);
    }
    //计算关节活动度
    calMotionScope();
}

//计算关节活动度结果
void MotionScopeEvaluation::calMotionScope()
{
    QMap<int,QPair<int,int>> fingersRange = ui->train_Widget->getFingersRange();
    QList<int> rangePercent;

    QMapIterator<int,QPair<int,int>> iter(fingersRange);

    //获取各关节活动度的百分比
    while(iter.hasNext())
    {
        iter.next();
        int percentValue = iter.value().second - iter.value().first;
        rangePercent.append(percentValue);
    }
    if(rangePercent.size() > 4)
    {
        ui->thumb_Label->setText(QString::number(rangePercent.at(0)));
        ui->forefinger_Label->setText(QString::number(rangePercent.at(1)));
        ui->middleFinger_Label->setText(QString::number(rangePercent.at(2)));
        ui->ringFinger_Label->setText(QString::number(rangePercent.at(3)));
        ui->littleFinger_Label->setText(QString::number(rangePercent.at(4)));

        //计算总的关节活动度
        int total = 0,fingerNum = 0;
        for(int i = 0;i < rangePercent.size();i++)
        {
            if(rangePercent.at(i) != 0)
            {
                total += rangePercent.at(i);
                fingerNum++;
            }
        }
        if(fingerNum != 0)
            ui->totalPercent_Label->setText(QString::number(total/fingerNum));
    }
}

//存储关节活动度评定结果
void MotionScopeEvaluation::saveMotionScopeResult()
{
    ST_JointRangeResult st_JointRangeResult;
    st_JointRangeResult.ID = CurrentUserData::getInstace()->getCurrentUserID();
    st_JointRangeResult.hand = st_HandData.hand;
    st_JointRangeResult.endDateTime = QDateTime::currentDateTime();
    st_JointRangeResult.startDateTime = m_StartDateTime;
    st_JointRangeResult.totalPercent = ui->totalPercent_Label->text().toInt();
    st_JointRangeResult.mapMovePercent.insert(0,ui->thumb_Label->text().toInt());
    st_JointRangeResult.mapMovePercent.insert(1,ui->forefinger_Label->text().toInt());
    st_JointRangeResult.mapMovePercent.insert(2,ui->middleFinger_Label->text().toInt());
    st_JointRangeResult.mapMovePercent.insert(3,ui->ringFinger_Label->text().toInt());
    st_JointRangeResult.mapMovePercent.insert(4,ui->littleFinger_Label->text().toInt());

    QString table("jointrangeresulttable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,jointRangeResultToVariantMap(st_JointRangeResult)))  
    {
//        logger()->debug(CDatabaseInterface::getInstance()->getLastError());
        qDebug()<<"关节活动度评定结果存储失败"<<CDatabaseInterface::getInstance()->getLastError();
    }

    st_trainReport.trainMode = TRAINMODE_SCOPE_E;
    st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
    st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;
    st_trainReport.level = 1;
    st_trainReport.startTime = m_StartDateTime;
    st_trainReport.endTime = QDateTime::currentDateTime();
    st_trainReport.duration = m_StartDateTime.secsTo(endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.resultImagePath = getImageName();
    st_trainReport.markMsg = QString(tr("总关节活动度%1")).arg(ui->totalPercent_Label->text().toInt());
    //将训练数据存入数据库
    QString table1("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table1,trainReportToVariantMap(st_trainReport)))
    {
//        logger()->debug(CDatabaseInterface::getInstance()->getLastError());
        qDebug()<<"关节活动度评定结果记录存储失败"<<CDatabaseInterface::getInstance()->getLastError();
    }
}

//手指状态发生变化
void MotionScopeEvaluation::slotFingerStateChanged(int8_t index,int8_t state)
{
    fingersState[index] = originalSteate[index] &state;

    for(int i = 0;i < 5;i++)
        ui->train_Widget->setHandFingerEnable(st_HandData.hand,i,fingersState[i]);
}

//重新评估
void MotionScopeEvaluation::on_reEvaluat_Btn_clicked()
{
    startMotionRangeEvaluate();

    for(int i = 0;i < 5;i++)
    {
        if(fingersState[i])
            ui->train_Widget->resetHandFingerValue(st_HandData.hand,i);
    }
}

//界面发生变化
void MotionScopeEvaluation::slotCurrentPageChanged(int index)
{
    //不再是当前页
    if(index != MotionScopeEvaluationPage_E)
    {
        disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    }
}

QString MotionScopeEvaluation::getImageName()
{
    //图片命名方法 ID+时间+描述 例：10010_20210910123456_scope.png
    QString dirPath = QApplication::applicationDirPath() + "/result";
    QDir resultDir(dirPath);
    if(!resultDir.exists())
        resultDir.mkdir(dirPath);

    QString fileName = QString::number(st_trainReport.ID) + "_" +
            m_StartDateTime.toString("yyyyMMddhhmm") +"_scope.png";
    QString absulatePath = dirPath + "/"  + fileName;

    return absulatePath;
}

void MotionScopeEvaluation::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == MotionScopeEvaluationPage_E)
    {
        on_finish_Btn_clicked();
    }
}

void MotionScopeEvaluation::slotCurrentUserChanged()
{
    //获取当前用户的手信息
    st_HandData = CurrentUserData::getInstace()->getCurrentUserHandData();
    //设置左右手
    ui->train_Widget->switchHand(st_HandData.hand);

    for(int i =0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
    }

    //初始化手指状态
    initHandFingers();
}

void MotionScopeEvaluation::clearSetAndParam()
{
    ui->buckking_Dial->setValue(0);
    ui->stretch_Dial->setValue(0);
    on_fourFinger_Btn_clicked();
    ui->thumb_Label->setText("0");
    ui->forefinger_Label->setText("0");
    ui->middleFinger_Label->setText("0");
    ui->ringFinger_Label->setText("0");
    ui->littleFinger_Label->setText("0");
    ui->totalPercent_Label->setText("0");
}
