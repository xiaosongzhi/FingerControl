#include "activetraining.h"
#include "ui_activetraining.h"
#include "ccommunicateapi.h"
#include <QDebug>
#include "currentuserdata.h"
#include <QMessageBox>
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include "readconfig.h"
ActiveTraining::ActiveTraining(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActiveTraining),
    m_controlSet(NULL),
    m_timeOutTimer(NULL),
    m_forceTimer(NULL),
    m_positionTimer(NULL),
    m_theRestTime(10000),
    m_rightTimes(0),
    m_errorTimes(0),
    m_timeOutTimes(0),
    m_isTrainning(false),
    m_currentFingerIndex(0),
    currentTime(0),
    m_isDisplayOn(false)
  //    testDirection(true)
{
    ui->setupUi(this);
    initWidget();
    m_controlSet = new ControlSet();
    m_timeOutTimer = new QTimer();
    m_forceTimer = new QTimer();
    m_positionTimer = new QTimer();
    m_forceTimer->setInterval(50);
    m_timeOutTimer->setInterval(50);
    m_positionTimer->setInterval(50);

    connect(ui->hand_Widget,&CHandStateShow::signalLabelOnPressed,this,&ActiveTraining::slotFingerPressed);
    connect(m_timeOutTimer,&QTimer::timeout,this,&ActiveTraining::slotTimerOut);
    connect(m_forceTimer,&QTimer::timeout,this,&ActiveTraining::slotChargeForceDirection);
    connect(m_positionTimer,&QTimer::timeout,this,&ActiveTraining::slotChargePosition);
    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    m_forceLevel = BASEFORCE;
    st_TrainParam.upDown = -1;
    st_TrainParam.isAuto = false;

    if(0 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(true);
    else if(1 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(false);

    testDirection = false;
}

ActiveTraining::~ActiveTraining()
{
    if(m_controlSet)
    {
        m_controlSet->deleteLater();
        m_controlSet = NULL;
    }
    if(m_timeOutTimer)
    {
        delete  m_timeOutTimer;
        m_timeOutTimer = NULL;
    }
    if(m_forceTimer)
    {
        delete  m_forceTimer;
        m_forceTimer = NULL;
    }
    if(m_positionTimer)
    {
        delete  m_positionTimer;
        m_positionTimer = NULL;
    }
    delete ui;
}

void ActiveTraining::initWidget()
{
    bucklingTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeNormal.png);}\n"
                                      "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeHover.png);}");
    bucklingTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeSelected.png);}");


    stretchTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeNormal.png);}\n"
                                     "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeHover.png);}");
    stretchTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeSelected.png);}");


    randomTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/randomTypeNormal.png);}\n"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/randomTypeHover.png);}");
    randomTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/randomTypeSelected.png);}");


    manualTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/manualTypeNormal.png);}\n"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/manualTypeHover.png);}");
    manualTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/manualTypeSelected.png);}");


    autoTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/autoTypeNormal.png);}\n"
                                  "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/autoTypeHover.png);}");
    autoTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/autoTypeSelected.png);}");


    ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.second);
    ui->manualType_Btn->setStyleSheet(manualTypeStyle.second);

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


void ActiveTraining::on_level1_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }

    if(ui->level1_Btn->styleSheet() == levelNormalList.at(0))
    {
        ui->level1_Btn->setStyleSheet(levelSelectedList.at(0));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
    sendTrainLevel(1);
    m_forceLevel = BASEFORCE;
}

void ActiveTraining::on_level2_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->level2_Btn->styleSheet() == levelNormalList.at(1))
    {
        ui->level2_Btn->setStyleSheet(levelSelectedList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
    sendTrainLevel(2);
    m_forceLevel = BASEFORCE*2;
}

void ActiveTraining::on_level3_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->level3_Btn->styleSheet() == levelNormalList.at(2))
    {
        ui->level3_Btn->setStyleSheet(levelSelectedList.at(2));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
    }
    sendTrainLevel(3);
    m_forceLevel = BASEFORCE*3;
}

void ActiveTraining::on_bucklingType_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->bucklingType_Btn->styleSheet() == bucklingTypeStyle.first)
    {
        ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.second);
        ui->stretchType_Btn->setStyleSheet(stretchTypeStyle.first);
        ui->randomType_Btn->setStyleSheet(randomTypeStyle.first);
    }
    st_TrainParam.upDown = -1;
}


void ActiveTraining::on_stretchType_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->stretchType_Btn->styleSheet() == stretchTypeStyle.first)
    {
        ui->stretchType_Btn->setStyleSheet(stretchTypeStyle.second);
        ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.first);
        ui->randomType_Btn->setStyleSheet(randomTypeStyle.first);
    }
    st_TrainParam.upDown = 1;
}

void ActiveTraining::on_randomType_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->randomType_Btn->styleSheet() == randomTypeStyle.first)
    {
        ui->randomType_Btn->setStyleSheet(randomTypeStyle.second);
        ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.first);
        ui->stretchType_Btn->setStyleSheet(stretchTypeStyle.first);
    }
    st_TrainParam.upDown = 2;
}

void ActiveTraining::on_manualType_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->manualType_Btn->styleSheet() == manualTypeStyle.first)
    {
        ui->manualType_Btn->setStyleSheet(manualTypeStyle.second);
        ui->autoType_Btn->setStyleSheet(autoTypeStyle.first);
    }
    st_TrainParam.isAuto = false;
    //在训练过程中调整是否使用鼠标点击
    if(m_isTrainning)
        ui->hand_Widget->switchPressedEvent(true);
}

void ActiveTraining::on_autoType_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    if(ui->autoType_Btn->styleSheet() == autoTypeStyle.first)
    {
        ui->autoType_Btn->setStyleSheet(autoTypeStyle.second);
        ui->manualType_Btn->setStyleSheet(manualTypeStyle.first);
    }
    st_TrainParam.isAuto = true;
    ui->hand_Widget->switchPressedEvent(false);
}

void ActiveTraining::on_controlSet_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("警告"));
        msgBox.setInformativeText(tr("训练过程中改变参数会结束当前训练"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch(ret)
        {
        case QMessageBox::Cancel:
            return;
            break;
        case QMessageBox::Ok:
            if(m_controlSet)
                m_controlSet->show();
            break;
        }
    }
    else
    {
        if(m_controlSet)
            m_controlSet->show();
    }

}

void ActiveTraining::on_display_Btn_clicked()
{
    if(m_isTrainning)
    {
        QMessageBox::warning(NULL,tr("提示"),tr("训练过程中不允许改变参数!"),QMessageBox::Ok);
        return;
    }
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    if(ui->display_Btn->styleSheet() == turnOn)
    {
        m_isDisplayOn = false;
        ui->display_Btn->setStyleSheet(turnOff);
    }
    else if(ui->display_Btn->styleSheet() == turnOff)
    {
        m_isDisplayOn = true;
        ui->display_Btn->setStyleSheet(turnOn);
    }
}

void ActiveTraining::on_start_Btn_clicked()
{
    if(ui->start_Btn->text() == tr("开始"))
    {
//        ui->start_Btn->setText("停止");
        ui->start_Btn->setEnabled(false);
        ui->start_Btn->setStyleSheet("QPushButton{background: #D3D3D3;border-radius: 8px;color:white;}");
        ui->trainTips_Label->setText(tr("训练中..."));
        MainWindowPageControl::getInstance()->updatePageRunningflag(ActiveTrainingPage_E,true);
        m_rightTimes = 0;
        m_errorTimes = 0;
        m_timeOutTimes = 0;
        m_isTrainning = true;
        currentTime = 0;
        //将手指延时显示打开
        ui->hand_Widget->switchFingerDelayShow(true);
        //获取训练参数
        ST_ControlParam st_ControlParam = m_controlSet->getControlParam();
        //默认进入力量模式
        m_currentMode = st_ControlParam.controlType;
        switchControlMode(st_ControlParam.controlType);

        //告知下位机开始训练
        Sleep(100);
        switchActiveTrain(true);
        //打开实时位置开关
        Sleep(100);
        if(2 == m_currentMode)
            CCommunicateAPI::getInstance()->getRealTimePositon(true);
        //打开实时力量开关
        else if(1 == m_currentMode)
            CCommunicateAPI::getInstance()->getRealTimeStrength(true);
        //若果是自动选择则直接开启,如果是鼠标点击则等待鼠标点击
        //判断是否打开点击开关(非自动)
        if(!st_TrainParam.isAuto)
        {
            ui->hand_Widget->switchPressedEvent(true);
        }
        if(st_TrainParam.isAuto)
            autoTrainProcess();

        startTime = QDateTime::currentDateTime();
        st_trainReport.startTime = startTime;
        st_trainReport.trainMode = TRAINMODE_ACTIVE_E;
        st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
        st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;
    }
    else if(ui->start_Btn->text() == tr("停止"))
    {
        MainWindowPageControl::getInstance()->updatePageRunningflag(ActiveTrainingPage_E,false);
        ui->start_Btn->setText(tr("开始"));
        m_isTrainning = false;
        ui->trainTips_Label->setText(tr("准备中..."));
        m_timeOutTimer->stop();
        m_positionTimer->stop();
        m_forceTimer->stop();
        //关闭实时位置
        CCommunicateAPI::getInstance()->getRealTimePositon(false);
        Sleep(100);
        //关闭实时力量
        CCommunicateAPI::getInstance()->getRealTimeStrength(false);
        //关闭主动训练
        Sleep(100);
        switchActiveTrain(false);
    }
}

void ActiveTraining::on_finish_Btn_clicked()
{
    ui->trainTips_Label->setText(tr("完成训练"));
    MainWindowPageControl::getInstance()->updatePageRunningflag(ActiveTrainingPage_E,false);
    m_rightTimes = 0;
    m_errorTimes = 0;
    m_timeOutTimes = 0;
    currentTime = 0;
    m_isTrainning = false;
    m_timeOutTimer->stop();
    m_positionTimer->stop();
    m_forceTimer->stop();

    ui->score_ProgressBar->setValue(0);
    ui->timeOut_ProgressBar->setValue(0);

    //将手指延时显示关闭
    ui->hand_Widget->switchFingerDelayShow(false);

    switchActiveTrain(false);
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimePositon(false);
    //打开实时力量开关
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimeStrength(false);
//    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));

    endTime = QDateTime::currentDateTime();
    st_trainReport.endTime  = endTime;
    st_trainReport.duration = startTime.secsTo(endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.markMsg = QString(tr("主动训练正确率为%1%")).arg(ui->score_ProgressBar->value());
    //将训练数据存入数据库
    QString table("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,trainReportToVariantMap(st_trainReport)))
    {
        qDebug()<<"主动训练结果存储失败";
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
    }
    else
        qDebug()<<"主动训练结果存储成功";

    MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);

    ui->start_Btn->setEnabled(true);
    ui->start_Btn->setStyleSheet("QPushButton{background: #0D9DDB;border-radius: 8px;color:white;}\nQPushButton:hover{background: #1EAACF;border-radius: 8px;color:#333333;}");
}

void ActiveTraining::on_timeLimit_SpinBox_valueChanged(int arg1)
{
    //时间，单位毫秒
    m_theRestTime = arg1*1000;
}

//训练流程
void ActiveTraining::autoTrainProcess()
{
    currentTime = 0;
    //自动生成手指下标
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int randNum = qrand()%5;
    m_currentFingerIndex = randNum;
    //先给出提示
    moveTipsCmd(m_currentFingerIndex,st_TrainParam.upDown);
    //再显示颜色
    ui->hand_Widget->setFingerState(m_currentFingerIndex,TipsPurple_E);
    //力量模式
    if(1 == m_currentMode)
    {
        qDebug()<<"力量检测开";
        m_forceTimer->start();
    }
    //位置模式
    else if(2 == m_currentMode)
    {
        m_positionTimer->start();
    }
}

void ActiveTraining::sendMoveDirectionCmd(ST_SetFingerPosiotion st_MovePosition)
{
    uint16_t cmdId = MOVE_FINGER_INDEX;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(ST_SetFingerPosiotion));
    memcpy(array.data(),&st_MovePosition,sizeof(ST_SetFingerPosiotion));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void ActiveTraining::sendTrainLevel(int8_t level)
{
    uint16_t cmdId = ACTIVE_TRAINNING_MODE_HELP_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&level,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void ActiveTraining::switchActiveTrain(bool isStart)
{
    uint16_t cmdId;
    if(isStart)
        cmdId = START_ACTIVE_TRANNING_MODE;
    else
        cmdId = CLOSE_ACTIVE_TRANNING_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//主动训练控制模式
void ActiveTraining::switchControlMode(int8_t mode)
{
    uint16_t cmdId = ACTIVE_TRANNING_MODE_CTRL_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&mode,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}
//从UDP接收数据
void ActiveTraining::receiveCommData(QByteArray receiveData)
{
    int length = receiveData[1];
    QByteArray data = receiveData.mid(5,length);
    uint16_t comID;
    memcpy(&comID,receiveData.data()+4,2);
    switch(comID)
    {
    case RESPONDPOSITION: //获取手指实时位置数据
    {
        ST_RealTimeFingersPosition fingersPosition;
        memcpy(&fingersPosition,receiveData.data() + 6,sizeof(ST_RealTimeFingersPosition));
        st_currentFingerPosition = fingersPosition;
//        ui->currThumb_label->setText(QString::number(fingersPosition.Lthumb));
//        ui->currFore_Label->setText(QString::number(fingersPosition.forefinger));
//        ui->currMidd_Label->setText(QString::number(fingersPosition.middlefinger));
//        ui->currRing_Label->setText(QString::number(fingersPosition.ringfinger));
//        ui->currLittle_Label->setText(QString::number(fingersPosition.littlefinger));

    }
        break;
    case RESPONDFORCE://获取手指实时力量数据
    {
        ST_RealTimeStrength st_RealTimeStrength;
        memcpy(&st_RealTimeStrength,receiveData.data() + 6,sizeof(ST_RealTimeStrength));
        st_currentFingerStrength = st_RealTimeStrength;
//        ui->forceThumb_label->setText(QString::number(st_RealTimeStrength.thumb));
//        ui->forceFore_Label->setText(QString::number(st_RealTimeStrength.forefinger));
//        ui->forceMidd_Label->setText(QString::number(st_RealTimeStrength.middlefinger));
//        ui->forceRing_Label->setText(QString::number(st_RealTimeStrength.ringfinger));
//        ui->forceLittle_Label->setText(QString::number(st_RealTimeStrength.littlefinger));
    }
        break;
    case START_ACTIVE_TRANNING_MODE://开始训练
        qDebug()<<"开始训练";
        break;
    case CLOSE_ACTIVE_TRANNING_MODE://停止训练
        qDebug()<<"停止训练";
        break;
    case ACTIVE_TRANNING_MODE_CTRL_TYPE://控制模式，1-位置 2-力量
        qDebug()<<"运动模式";
        break;
    case ACTIVE_TRAINNING_MODE_HELP_TYPE://难度等级
        qDebug()<<"难度等级";
        break;
    case ACTIVE_TRAINNING_MODE_COMPLETE_TIPS://完成运动提示
    {
        qDebug()<<"完成运动提示";
        //恢复颜色
        //        ui->hand_Widget->setFingerState(m_currentFingerIndex,NORMAL_E);
        //开启倒计时

    }
        break;
    }
}

void ActiveTraining::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    m_isTrainning = false;
    ui->trainTips_Label->setText(tr("准备中..."));
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->setHandType(m_handType);

    //关节鼠标点击事件(在开始训练并且为点击状态下才开启)
    ui->hand_Widget->switchPressedEvent(false);
    //设置手指状态
    //    ui->hand_Widget->setFingerEnabled();
    //告知下位机进入主动运动模式
    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_ACTIVE_E);
    //获取边界值
    st_bucklingPosition = CurrentUserData::getInstace()->getMinDestPosition();
    st_stretchPosition = CurrentUserData::getInstace()->getMaxDestPosition();
    //获取手指状态
    ST_HandData st_handData;
    st_handData = CurrentUserData::getInstace()->getCurrentUserHandData();
    for(int i =0;i < 5;i++)
    {
        fingerEnable[i] = st_handData.fingerSelected[i];
    }
    updateFingersEnableState(fingerEnable);

    //打开显示提示
    on_display_Btn_clicked();
}

void ActiveTraining::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    //告知下位机控制模式
}

//给出运动提示
void ActiveTraining::moveTipsCmd(int index,int8_t direction)
{
    uint16_t cmdId = ACTIVE_TRAINNING_MODE_TIPS;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    ST_FingerUpDownControl st_fingerUpDownControl;
    st_fingerUpDownControl.fingerIndex = index;
    //随机生成方向
    if(direction == 2)
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int randNum = qrand()%9;
        if(randNum%2)
            st_fingerUpDownControl.direction = -1;
        else
            st_fingerUpDownControl.direction = 1;
    }
    else
        st_fingerUpDownControl.direction = direction;

    m_currentDirection = st_fingerUpDownControl.direction;
    array.resize(sizeof(ST_FingerUpDownControl));
    memcpy(array.data(),&st_fingerUpDownControl,sizeof(st_fingerUpDownControl));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);

    m_timeOutTimer->start();
}

void ActiveTraining::slotFingerPressed(QString fingerName)
{
    currentTime = 0;
    m_currentFingerIndex = fingerName.right(1).toInt();
    //先给出提示
    moveTipsCmd(m_currentFingerIndex,st_TrainParam.upDown);
    //再显示颜色
    if(m_isDisplayOn)
        ui->hand_Widget->setFingerState(m_currentFingerIndex,TipsPurple_E);
    //力量模式
    if(1 == m_currentMode)
        m_forceTimer->start();
    //位置模式
    else if(2 == m_currentMode)
    {
        m_positionTimer->start();
    }
}

//超时
void ActiveTraining::slotTimerOut()
{
    currentTime+=50;
    ui->timeOut_ProgressBar->setValue((float)currentTime/m_theRestTime*100);
    if(currentTime >= m_theRestTime)
    {
        ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateYellow_E);
        m_timeOutTimes++;
        currentTime = 0;
        m_timeOutTimer->stop();
        //结束当前流程
        m_forceTimer->stop();
        m_positionTimer->stop();
        ui->timeOut_ProgressBar->setValue(0);

        //当是位置模式时回到原位置
        if(2 == m_currentMode)
            rebackToOriginalPosition();

        //如果是自动运动则直接开启下一流程
        if(st_TrainParam.isAuto)
        {
            Sleep(3000);
            autoTrainProcess();
        }
    }
}

//力量模式下判断运动是否正确
void ActiveTraining::slotChargeForceDirection()
{
    //判断当前手指是否按照制定方向用力,并且力量达到规定值
    int currentForce = 0;

    switch(m_currentFingerIndex)
    {
    case Thumb_E:
        currentForce = st_currentFingerStrength.thumb;
        break;
    case ForeFinger_E:
        currentForce = st_currentFingerStrength.forefinger;
        break;
    case MiddleFinger_E:
        currentForce = st_currentFingerStrength.middlefinger;
        break;
    case RingFinger_E:
        currentForce = st_currentFingerStrength.ringfinger;
        break;
    case LittleFinger_E:
        currentForce = st_currentFingerStrength.littlefinger;
        break;
    }
//    ui->currentForce_Label->setText(QString::number(currentForce));
//    ui->destForce_Label->setText(QString::number(m_forceLevel));
    //手指出现运动趋势
    if(qAbs(currentForce) > m_forceLevel)
    {
        //判断方向
        int currDirection = 0;
        if(currentForce < 0)
            currDirection = 1;
        else if(currentForce > 0)
            currDirection = -1;

        if(m_currentDirection == currDirection)
        {
            m_rightTimes++;
            qDebug()<<"正确";
            ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateGreen_E);
        }
        else
        {
            m_errorTimes++;
            ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateRed_E);
            qDebug()<<"错误";
        }
        //当前流程结束
        m_forceTimer->stop();
        m_timeOutTimer->stop();
        ui->timeOut_ProgressBar->setValue(0);

        ui->score_ProgressBar->setValue((float)m_rightTimes/(m_rightTimes+m_errorTimes + m_timeOutTimes)*100);

        //如果是自动运动则直接开启下一流程
        if(st_TrainParam.isAuto)
        {
            Sleep(3000);
            autoTrainProcess();
        }
    }

}

//位置模式下判断是否正确
void ActiveTraining::slotChargePosition()
{
    int destposition = 0;//正确目标位置
    int oppositePosition = 0;//反向目标位置
    switch(m_currentDirection)
    {
    case Buckling_E:
        if(testDirection)
        {
            destposition = st_bucklingPosition.array[m_currentFingerIndex];
            oppositePosition = st_stretchPosition.array[m_currentFingerIndex];
        }
        else
        {
            oppositePosition = st_bucklingPosition.array[m_currentFingerIndex];
            destposition = st_stretchPosition.array[m_currentFingerIndex];
        }
        break;
    case Stretch_E:
        if(testDirection)
        {
            destposition = st_stretchPosition.array[m_currentFingerIndex];
            oppositePosition = st_bucklingPosition.array[m_currentFingerIndex];
        }
        else
        {
            oppositePosition = st_stretchPosition.array[m_currentFingerIndex];
            destposition = st_bucklingPosition.array[m_currentFingerIndex];
        }
        break;
    }

    int currentPosition;
    switch(m_currentFingerIndex)
    {
    case Thumb_E:
        switch(m_handType)
        {
        case LeftHand_E:
            currentPosition = st_currentFingerPosition.Lthumb;
            break;
        case RightHand_E:
            currentPosition = st_currentFingerPosition.Rthumb;
            break;
        }
        break;
    case ForeFinger_E:
        currentPosition = st_currentFingerPosition.forefinger;
        break;
    case MiddleFinger_E:
        currentPosition = st_currentFingerPosition.middlefinger;
        break;
    case RingFinger_E:
        currentPosition = st_currentFingerPosition.ringfinger;
        break;
    case LittleFinger_E:
        currentPosition = st_currentFingerPosition.littlefinger;
        break;
    }

//    ui->currentPosition_Label->setText(QString::number(currentPosition));
//    ui->destPosition_Label->setText(QString::number(oppositePosition));

    //运动正确
    if(qAbs(oppositePosition - currentPosition) < 2)
    {
        //回到初始位置
        rebackToOriginalPosition();
        m_rightTimes++;
        //结束当前流程
        m_positionTimer->stop();
        m_timeOutTimer->stop();
        ui->timeOut_ProgressBar->setValue(0);
        ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateGreen_E);

        ui->score_ProgressBar->setValue((float)m_rightTimes/(m_rightTimes+m_errorTimes + m_timeOutTimes)*100);
        //如果是自动运动则直接开启下一流程
        if(st_TrainParam.isAuto)
        {
            Sleep(3000);
            autoTrainProcess();
        }
    }
    //运动错误
    else if(qAbs(destposition - currentPosition) < 2)
    {
        //回到初始位置
        rebackToOriginalPosition();
        m_errorTimes++;
        //结束当前流程
        m_positionTimer->stop();
        m_timeOutTimer->stop();
        ui->timeOut_ProgressBar->setValue(0);
        ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateRed_E);

        ui->score_ProgressBar->setValue((float)m_rightTimes/(m_rightTimes+m_errorTimes + m_timeOutTimes)*100);
        //如果是自动运动则直接开启下一流程
        if(st_TrainParam.isAuto)
        {
            Sleep(3000);
            autoTrainProcess();
        }
    }
}

//位置模式回到初始位置
void ActiveTraining::rebackToOriginalPosition()
{
    uint16_t cmdId = ACTIVE_TRAINNING_MODE_REBACK;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&m_currentFingerIndex,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void ActiveTraining::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == ActiveTrainingPage_E)
    {
        qDebug()<<"关闭主动训练界面";
        on_finish_Btn_clicked();
    }
}

void ActiveTraining::updateFingersEnableState(int8_t* enableStates)
{
    for(int i = 0;i < 5;i++)
    {
        ui->hand_Widget->setFingerEnabled(i,enableStates[i]);
    }
}

void ActiveTraining::slotCurrentUserChanged()
{
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->setHandType(m_handType);
    st_bucklingPosition = CurrentUserData::getInstace()->getMinDestPosition();
    st_stretchPosition = CurrentUserData::getInstace()->getMaxDestPosition();
    //获取手指的选中状态
    ST_HandData st_handData;
    st_handData = CurrentUserData::getInstace()->getCurrentUserHandData();
    for(int i =0;i < 5;i++)
    {
        fingerEnable[i] = st_handData.fingerSelected[i];
    }
    updateFingersEnableState(fingerEnable);
}

void ActiveTraining::setTestVisible(bool isShow)
{
//    ui->test_GroupBox->setVisible(isShow);
}
