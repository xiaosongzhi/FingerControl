#include "boosttraining.h"
#include "ui_boosttraining.h"
#include "ccommunicateapi.h"
#include <QDebug>
#include "currentuserdata.h"
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include "readconfig.h"
#include <QMessageBox>

BoostTraining::BoostTraining(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoostTraining),
    m_processTimer(NULL),
    m_isTrainingCompletedTimer(NULL),
    m_progressBarTimer(NULL),
    m_handType(0),      //左手
    m_fingerType(1),    //四指
    m_destFunction(1),
    m_isArrivedDest(true),
    m_destDuration(0),
    m_trainTimes(0),
    currentDuration(0),
    currentTrainTimes(0),
    currentPositionFlag(1),
    countType(1),
    totalTrainTime(0),
    isUsedBoost(false),
    currentTime(0),
    totalTimeout(8000),
    m_isTraining(false)
{
    ui->setupUi(this);
    initWidget();

    m_processTimer = new QTimer();
    connect(m_processTimer,SIGNAL(timeout()),this,SLOT(slotTimerCheckProcess()));
    connect(ui->hand_Widget,&CTrainingHand::signalFingerStateChanged,this,&BoostTraining::slotFingerStateChanged);
    //设置当前手的训练状态为助力
    ui->hand_Widget->setMode(TRAINMODE_BOOST_E);

    //设置成单次触发定时器,用来检测手指是否到达目标位置
    m_processTimer->setInterval(totalTimeout);
    m_processTimer->setSingleShot(true);
    m_isTrainingCompletedTimer = new QTimer();
    m_isTrainingCompletedTimer->setInterval(100);
    connect(m_isTrainingCompletedTimer,SIGNAL(timeout()),this,SLOT(slotTimerTrainingCompleted()));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    ui->currentTimes_Label->setText("0");
    ui->currentDuration_Label->setText("0");

    ui->buckling_ProgressBar->setValue(0);
    ui->stretch_ProgressBar->setValue(0);

    //初始化曲线
    ui->chart_Widget->initChart(LineChart_E,tr("力量曲线"));

    if(0 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(true);
    else if(1 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(false);

    m_progressBarTimer = new QTimer();
    m_progressBarTimer->setInterval(25);
    connect(m_progressBarTimer,SIGNAL(timeout()),this,SLOT(slotTimeOutProgressbar()));
}

BoostTraining::~BoostTraining()
{
    if(m_processTimer)
    {
        delete  m_processTimer;
        m_processTimer = NULL;
    }
    delete ui;
}

void BoostTraining::initWidget()
{
    boostFourNormal = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostFourNoSelected.png);}\n"
                      "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/boostFourHover.png);}";

    boostFourSelected = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostFourSelected.png);}";

    boostOneNormal = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostOneNoSelected.png);}\n"
                     "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/boostOneHover.png);}";
    boostOneSelected = "QPushButton{border-image: url(:/new/prefix1/source/buttons/level/boostOneSelected.png);}";

    ui->fourFinger_Btn->setStyleSheet(boostFourSelected);

    for(int i = 0;i < 3;i++)
    {
        QString selected = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1Selected.png);}").arg(i+1);;
        levelSelectedList.append(selected);
        QString normal = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1NoSelect.png);}\n"
                                 "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/level%2Hover.png);}").arg(i+1).arg(i+1);
        levelNormalList.append(normal);
    }
    ui->level1_Btn->setStyleSheet(levelSelectedList.at(0));
}

void BoostTraining::on_fourFinger_Btn_clicked()
{
    if(ui->fourFinger_Btn->styleSheet() == boostFourNormal)
    {
        //选中四指
        m_fingerType = 1;
        setMoveFinger(1);
        ui->fourFinger_Btn->setStyleSheet(boostFourSelected);
        ui->oneFinger_Btn->setStyleSheet(boostOneNormal);
    }
}

void BoostTraining::on_oneFinger_Btn_clicked()
{
    if(ui->oneFinger_Btn->styleSheet() == boostOneNormal)
    {
        //选中拇指
        m_fingerType = 0;
        setMoveFinger(0);
        ui->oneFinger_Btn->setStyleSheet(boostOneSelected);
        ui->fourFinger_Btn->setStyleSheet(boostFourNormal);
    }
}

void BoostTraining::on_level1_Btn_clicked()
{
    if(ui->level1_Btn->styleSheet() == levelNormalList.at(0))
    {
        setBoostTrainLevel(1);
        ui->level1_Btn->setStyleSheet(levelSelectedList.at(0));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
}

void BoostTraining::on_level2_Btn_clicked()
{
    if(ui->level2_Btn->styleSheet() == levelNormalList.at(1))
    {
        setBoostTrainLevel(2);
        ui->level2_Btn->setStyleSheet(levelSelectedList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
}

void BoostTraining::on_level3_Btn_clicked()
{
    if(ui->level3_Btn->styleSheet() == levelNormalList.at(2))
    {
        setBoostTrainLevel(3);
        ui->level3_Btn->setStyleSheet(levelSelectedList.at(2));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
    }
}

//设置训练手指(四指或者拇指)
void BoostTraining::setMoveFinger(int8_t fingerType)
{
    uint16_t cmdId = ASSISTANCE_MODE_FINGER_SELECT;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&fingerType,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//设置助力训练等级
void BoostTraining::setBoostTrainLevel(int8_t level)
{
    uint16_t cmdId = ASSISTANCE_MODE_HELP_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&level,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//助力训练开关
void BoostTraining::switchBoostTrain(bool isStart)
{
    uint16_t cmdId;
    if(isStart)
        cmdId = START_ASSISTANCE_MODE;
    else
        cmdId = CLOSE_ASSISTANCE_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void BoostTraining::receiveCommData(QByteArray receiveData)
{
    if(m_isTraining)
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
            if(0 == m_handType)
                st_handPosition.thumb =100- st_RealTimePosition.Lthumb;
            else if(1 == m_handType)
                st_handPosition.thumb =100- st_RealTimePosition.Rthumb;
            st_handPosition.forefinger = st_RealTimePosition.forefinger;
            st_handPosition.middlefinger =st_RealTimePosition.middlefinger;
            st_handPosition.ringfinger =st_RealTimePosition.ringfinger;
            st_handPosition.littlefinger =st_RealTimePosition.littlefinger;
            setHandFingersPosition(st_handPosition);
            //检查是否主动到达目标位置
            if( chargeIsArrivedDest(st_RealTimePosition) /*&& m_isArrivedDest*/)
            {
                currentTrainTimes++;
                if(currentTrainTimes > ui->times_SpinBox->value())
                    currentTrainTimes = ui->times_SpinBox->value();
                ui->currentTimes_Label->setText(QString::number(currentTrainTimes));
                //停止当前流程计时器
                m_processTimer->stop();
                qDebug()<<"到达目标位置"<<"-----开启单流程";
                //使用助力
                if(isUsedBoost)
                    showScopeChart();
                else
                {
                    if(m_destFunction == 1)
                        stretchList.append(ui->stretch_ProgressBar->value());
                    if(m_destFunction == -1)
                        bucklingList.append(ui->buckling_ProgressBar->value());
                    showScopeChart();
                }

                startTrainProcess();
            }
        }
            break;
        case ASSISTANCE_REACH_TO_DEST://给助力的情况下到达到达指定位置
        {
            //下位机告知达到目标位置
            //m_isArrivedDest = true;
            //qDebug()<<"被动到达目标位置--下位机告知已到达目标位置"<<"-----开启单流程";
            //startTrainProcess();
        }
            break;
        }
    }

}

void BoostTraining::showTempData(ST_RealTimeFingersPosition& fingersPosition)
{
//    ui->currThumb_label->setText(QString::number(st_handPosition.thumb));
//    ui->currFore_Label->setText(QString::number(fingersPosition.forefinger));
//    ui->currMidd_Label->setText(QString::number(fingersPosition.middlefinger));
//    ui->currRing_Label->setText(QString::number(fingersPosition.ringfinger));
//    ui->currLittle_Label->setText(QString::number(fingersPosition.littlefinger));

//    ui->destThumb_label->setText(QString::number(st_DestPosition.thumb));
//    ui->destFore_Label->setText(QString::number(st_DestPosition.forefinger));
//    ui->destMidd_Label->setText(QString::number(st_DestPosition.middlefinger));
//    ui->destRing_Label->setText(QString::number(st_DestPosition.ringfinger));
//    ui->destLittle_Label->setText(QString::number(st_DestPosition.littlefinger));
}

//检测是否达到目标位置
bool BoostTraining::chargeIsArrivedDest(ST_RealTimeFingersPosition& st_RealTimePosition)
{
    showTempData(st_RealTimePosition);
    int8_t result[5] = {0};

    if(m_fingerType == 0)
    {
        int value = 0;
        if(0 == m_handType)
        {
            value = qAbs(st_RealTimePosition.Lthumb - st_DestPosition.thumb);
            if(value < DEADLINE)
                result[0] = 1;
        }
        else if(1 == m_handType)
        {
            value = qAbs(st_RealTimePosition.Rthumb - st_DestPosition.thumb);
            if(value < DEADLINE)
                result[0] = 1;
        }
        setStrengthShow(m_destFunction,(float)value/m_oneTotalScope*100);
    }
    else if(m_fingerType == 1)
    {
        int tempSum = 0;
        tempSum += qAbs(st_RealTimePosition.forefinger-st_DestPosition.forefinger);
        if(qAbs(st_RealTimePosition.forefinger-st_DestPosition.forefinger)<DEADLINE)
        {
            result[1] = 1;
        }
        tempSum += qAbs(st_RealTimePosition.middlefinger - st_DestPosition.middlefinger);
        if(qAbs(st_RealTimePosition.middlefinger - st_DestPosition.middlefinger) < DEADLINE)
        {

            result[2] = 1;
        }
        tempSum += qAbs(st_RealTimePosition.ringfinger - st_DestPosition.ringfinger);
        if(qAbs(st_RealTimePosition.ringfinger - st_DestPosition.ringfinger)<DEADLINE)
        {

            result[3] = 1;
        }
        tempSum += qAbs(st_RealTimePosition.littlefinger - st_DestPosition.littlefinger);
        if(qAbs(st_RealTimePosition.littlefinger - st_DestPosition.littlefinger) < DEADLINE)
        {

            result[4] = 1;
        }
        setStrengthShow(m_destFunction,(float)tempSum/m_fourTotalScope*100);
    }

    int sum = 0;
    for(int i = 0;i < 5;i++)
    {
        sum += result[i];
    }
    //检测四指
    if(m_fingerType == 1)
    {
        int tempSum = 0;
        for(int i = 1;i < 5;i++)
            tempSum += fingersState[i];
        if(tempSum == sum)
        {
            qDebug()<<"四指检测正确"<<tempSum<<sum;
            return true;
        }
    }
    //检测拇指
    if(m_fingerType == 0 && result[0] == 1 && fingersState[0])
    {
        qDebug()<<"拇指检测正确";
        return true;
    }
    return false;
}

void BoostTraining::setHandFingersPosition(ST_HandFingersPosition st_FingerPosition)
{
    ui->hand_Widget->setHandFingerValue(m_handType,0,st_FingerPosition.thumb);
    ui->hand_Widget->setHandFingerValue(m_handType,1,st_FingerPosition.forefinger);
    ui->hand_Widget->setHandFingerValue(m_handType,2,st_FingerPosition.middlefinger);
    ui->hand_Widget->setHandFingerValue(m_handType,3,st_FingerPosition.ringfinger);
    ui->hand_Widget->setHandFingerValue(m_handType,4,st_FingerPosition.littlefinger);
}

void BoostTraining::on_start_Btn_clicked()
{
    if(ui->start_Btn->text() == tr("开始"))
    {
        ui->start_Btn->setEnabled(false);
        ui->start_Btn->setStyleSheet("QPushButton{background: #D3D3D3;border-radius: 8px;color:white;}");
        m_isTraining = true;
        MainWindowPageControl::getInstance()->updatePageRunningflag(BoostTrainingPage_E,true);
        ui->trainTips_Label->setText(tr("训练中..."));

        //开启获取实时位置
        switchBoostTrain(true);
        Sleep(100);
        setMoveFinger(m_fingerType);
        Sleep(100);
        setBoostTrainLevel(1);
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimePositon(true);

        startTrainProcess();
        m_isTrainingCompletedTimer->start();
        totalTrainTime = 0;

        stretchList.clear();
        bucklingList.clear();

        startTime = QDateTime::currentDateTime();
        st_trainReport.startTime = startTime;
        st_trainReport.trainMode = TRAINMODE_BOOST_E;
        st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
        st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;
    }
    else if(ui->start_Btn->text() == tr("停止"))
    {
        ui->start_Btn->setText(tr("开始"));
        m_isTraining = false;
        ui->trainTips_Label->setText(tr("准备中..."));
        m_processTimer->stop();
        m_progressBarTimer->stop();
        m_isTrainingCompletedTimer->stop();
        switchBoostTrain(false);
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimePositon(false);
    }
}

void BoostTraining::on_finish_Btn_clicked()
{
    m_isTraining = false;
    ui->trainTips_Label->setText(tr("训练结束..."));
    MainWindowPageControl::getInstance()->updatePageRunningflag(BoostTrainingPage_E,false);
    //结束训练
    m_processTimer->stop();
    m_progressBarTimer->stop();
    currentTime = 0;

    m_isTrainingCompletedTimer->stop();
//    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    switchBoostTrain(false);
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimePositon(false);

    endTime = QDateTime::currentDateTime();
    st_trainReport.endTime  = endTime;
    st_trainReport.duration = startTime.secsTo(endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.markMsg = QString(tr("共进行了%1次抓握训练")).arg(currentTrainTimes);

    currentTrainTimes = 0;
    //将训练数据存入数据库
    QString table("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,trainReportToVariantMap(st_trainReport)))
    {
        qDebug()<<"助力训练结果存储失败";
    }
    MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);

    ui->start_Btn->setEnabled(true);
    ui->start_Btn->setStyleSheet("QPushButton{background: #0D9DDB;border-radius: 8px;color:white;}\nQPushButton:hover{background: #1EAACF;border-radius: 8px;color:#333333;}");
}

void BoostTraining::moveToDestLimitPosition(ST_PassiveDestPosition st_passiveDestPosition)
{
    //上位机控制到达目标位置
    m_isArrivedDest = false;
    uint16_t cmdId = ASSISTANCE_MOVE_DEST_POSITION;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(st_passiveDestPosition));
    memcpy(array.data(),&st_passiveDestPosition,sizeof(ST_PassiveDestPosition));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void BoostTraining::startTrainProcess()
{
    Sleep(500);
    isUsedBoost = false;
    //清除力量显示
    ui->buckling_ProgressBar->setValue(0);
    ui->stretch_ProgressBar->setValue(0);
    //根据当前位置设置目标位置(替换目标位置)
    if(m_destFunction == 1)
        m_destFunction = -1;
    else if(m_destFunction == -1)
        m_destFunction = 1;
    //显示运动方向
    showMoveDirection(m_destFunction);
    //开始记录当前小流程的时间
    m_processTimer->start();
    m_progressBarTimer->start();
    currentTime = 0;

}

void BoostTraining::slotTimerTrainingCompleted()
{
    //按次计数
    if(1 == countType)
    {
        m_trainTimes = ui->times_SpinBox->text().toInt();
        if(currentTrainTimes >= m_trainTimes)
        {
            //结束训练
            currentTrainTimes = 0;
            ui->currentTimes_Label->setText("0");
            QMessageBox::warning(NULL,tr("提示"),tr("达到设定抓握次数，训练停止"));
            on_finish_Btn_clicked();
            qDebug()<<"次数到达，停止训练";
        }
    }
    else if(2 == countType)//按时间统计
    {
        //当前进入的次数
        currentDuration++;
        //由时间转换成次数,周期为100ms
        m_destDuration = ui->duration_SpinBox->text().toInt()*60*10;
        if(currentDuration >= m_destDuration)
        {
            //结束训练
            currentDuration = 0;
            QMessageBox::warning(NULL,tr("提示"),tr("时间到达，停止训练"));
            on_finish_Btn_clicked();
            qDebug()<<"时间到达，停止训练";
            ui->currentDuration_Label->setText("0");
        }
    }
    totalTrainTime+=100;
    ui->currentDuration_Label->setText(QString::number(totalTrainTime/60000));
}

//超时启动助力
void BoostTraining::slotTimerCheckProcess()
{
    //时间已到，但是手指未移动到位的情况下告知下位机运动到制定位置
    qDebug()<<"时间已到，但是手指未移动到位的情况下告知下位机运动到制定位置";
    ///此时获取关节活动数据进行绘图
    //时间超时开启助力
    isUsedBoost = true;
    if(m_destFunction == 1)
        stretchList.append(ui->stretch_ProgressBar->value());
    if(m_destFunction == -1)
        bucklingList.append(ui->buckling_ProgressBar->value());
    showScopeChart();
    st_passiveDestPosition.flag = m_destFunction;
    moveToDestLimitPosition(st_passiveDestPosition);
}

void BoostTraining::on_times_RadioButton_clicked()
{
    countType = 1;
}

void BoostTraining::on_duration_RadioButton_clicked()
{
    countType = 2;
}

//显示运动方向
void BoostTraining::showMoveDirection(int8_t flag)
{
    if(-1 == flag)
    {
        ui->direction_Label->setStyleSheet("border-image: url(:/new/prefix1/source/image/bluckling.png);");
        st_DestPosition = CurrentUserData::getInstace()->getMinDestPosition();
    }
    else if(1 == flag)
    {
        ui->direction_Label->setStyleSheet("border-image: url(:/new/prefix1/source/image/stretch.png);");
        st_DestPosition = CurrentUserData::getInstace()->getMaxDestPosition();
    }
}

void BoostTraining::showEvent(QShowEvent *event)
{
    ui->trainTips_Label->setText(tr("准备中..."));
    Q_UNUSED(event)

    ui->stretch_ProgressBar->setValue(0);
    ui->buckling_ProgressBar->setValue(0);

    //1.设置左右手
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->switchHand(m_handType);
    //2.更改箭头显示位置
    if(0 == m_handType)
        ui->direction_Label->move(160,364);
    else if(1 == m_handType)
        ui->direction_Label->move(370,364);
    //3.告知下位机进入助力模式
    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_BOOST_E);

    //设置手指的活动基本范围
    ui->hand_Widget->setBaseMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //设置手指活动实际范围
    ui->hand_Widget->setRealMoveScope(CurrentUserData::getInstace()->getOppositeRange());

    //初始化手指
    for(int i = 0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        st_passiveDestPosition.array[i] = originalSteate[i];
        for(int i = 0;i < 5;i++)
            ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
    }
    //计算屈曲和伸展的最大值
    calculateStretchAndBluckling();
}

//显示屈曲和伸展状态
void BoostTraining::calculateStretchAndBluckling()
{
    m_oneTotalScope = 0;
    m_fourTotalScope = 0;

    ST_DestPosition st_max =  CurrentUserData::getInstace()->getMaxDestPosition();
    ST_DestPosition st_min =  CurrentUserData::getInstace()->getMinDestPosition();

    //计算拇指的运动范围
    if(fingersState[0])
        m_oneTotalScope = qAbs(st_max.thumb - st_min.thumb);
    //计算四指总运动范围
    if(fingersState[1])
        m_fourTotalScope+=qAbs(st_max.forefinger - st_min.forefinger);
    if(fingersState[2])
        m_fourTotalScope+=qAbs(st_max.middlefinger - st_min.middlefinger);
    if(fingersState[3])
        m_fourTotalScope+=qAbs(st_max.ringfinger - st_min.ringfinger);
    if(fingersState[4])
        m_fourTotalScope+=qAbs(st_max.littlefinger - st_min.littlefinger);
}

//设置力量显示
void BoostTraining::setStrengthShow(int8_t type,uint8_t strengthPercent)
{
    int percentValue = 100-strengthPercent;
    if(percentValue < 0)
        percentValue = 0;
    if(percentValue > 100)
        percentValue = 100;
    switch(type)
    {
    case -1://屈曲(向上 力量为负值)
        ui->buckling_ProgressBar->setValue(percentValue);
        //        ui->buckling_Label->setText(QString::number(percentValue) + "%");
        break;
    case 1://伸展(向下 力量为正直)
        ui->stretch_ProgressBar->setValue(percentValue);
        //        ui->stretch_Label->setText(QString::number(percentValue) + "%");
        break;
    }
}

//手指状态发生变化
void BoostTraining::slotFingerStateChanged(int8_t index,int8_t state)
{
    //更新当前手指状态
    fingersState[index] = originalSteate[index] & state;
    st_passiveDestPosition.array[index] = fingersState[index];
    for(int i = 0;i < 5;i++)
        ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
}

//关节活动度曲线
void BoostTraining::showScopeChart()
{
    if(stretchList.size() > 100)
        stretchList.removeFirst();
    if(bucklingList.size() > 100)
        bucklingList.removeFirst();

    //绘制伸展和屈曲曲线
    DataTable dataTable;
    DataList s_dataList;
    DataList b_dataList;
    QPair<DataList,QColor> dataColorPairS;
    QPair<DataList,QColor> dataColorPairB;
    for(int i = 1;i < stretchList.size();i++)
    {
        Data s_tempData;
        Data b_tempData;
        s_tempData.first.setX(i-1);
        s_tempData.first.setY(stretchList.at(i-1));
        b_tempData.first.setX(i-1);
        b_tempData.first.setY(bucklingList.at(i-1));
        s_dataList.append(s_tempData);
        b_dataList.append(b_tempData);
    }
    dataColorPairS.first = s_dataList;
    dataColorPairS.second = QColor("#C495F5");
    dataColorPairB.first = b_dataList;
    dataColorPairB.second = QColor("#70E086");
    dataTable.append(dataColorPairS);
    dataTable.append(dataColorPairB);
    ui->chart_Widget->setChartData(LineChart_E,dataTable);
    ui->chart_Widget->setAxisValue(Y_Axis,0,100);
}

void BoostTraining::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == BoostTrainingPage_E)
    {
        qDebug()<<"关闭助力训练界面";
        on_finish_Btn_clicked();
    }
}

void BoostTraining::slotCurrentUserChanged()
{
    //1.设置左右手
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->switchHand(m_handType);
    //2.更改箭头显示位置
    if(0 == m_handType)
        ui->direction_Label->move(160,364);
    else if(1 == m_handType)
        ui->direction_Label->move(370,364);

    //设置手指的活动基本范围
    ui->hand_Widget->setBaseMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //设置手指活动实际范围
    ui->hand_Widget->setRealMoveScope(CurrentUserData::getInstace()->getOppositeRange());

    //初始化手指
    for(int i = 0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        st_passiveDestPosition.array[i] = originalSteate[i];
        for(int i = 0;i < 5;i++)
            ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
    }
}

void BoostTraining::setTestVisible(bool isShow)
{       
//    ui->test_GroupBox->setVisible(isShow);
}

void BoostTraining::slotTimeOutProgressbar()
{
    currentTime += 25;
    ui->timeOut_ProgressBar->setValue(((float)currentTime)/totalTimeout*100);
    //超时
    if(currentTime >=  totalTimeout)
    {
        m_progressBarTimer->stop();
        currentTime = 0;
    }
}

void BoostTraining::on_stretch_ProgressBar_valueChanged(int value)
{
    ui->stretch_Label->setText(QString("%1").arg(value));
}

void BoostTraining::on_buckling_ProgressBar_valueChanged(int value)
{
    ui->buckling_Label->setText(QString("%1").arg(value));
}
