#include "passivetraining.h"
#include "ui_passivetraining.h"
#include "dataFormate.h"
#include "ccommunicateapi.h"
#include <QDebug>
#include "currentuserdata.h"
#include <QMapIterator>
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include <QMessageBox>
#include "readconfig.h"
PassiveTraining::PassiveTraining(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassiveTraining),
    m_fingerDirection(-1),//小指到拇指
    m_actionType(-1), //拇指同时运动
    m_moveDirection(-1),//手指初始位置为伸展位置，目标方向为屈曲方向
    m_fingerInterval(10),
    m_processDelay(0),
    m_currentDirection(-1),
    m_positionTimer(NULL),
    m_timeOutTimer(NULL),
    m_finishTimer(NULL),
    timeOutTime(5000), //起始设置为5000ms
    m_scopePercent(1),
    m_totalBucklingStrength(0),
    m_totalStretchStrength(0),
    m_isArrivedDestPosition(false),
    m_lastFingerPosition(0),
    m_isTraining(false),
    m_trainProcessType(-1),
    m_lastSpeed(2)
{
    ui->setupUi(this);
    initWidget();
    ui->verticalScrollBar->setRange(0,0);
    ui->leftToRight_Btn->onClicked();
    ui->allFingers_Btn->onClicked();
    m_positionTimer = new QTimer();
    m_positionTimer->setInterval(50);
    connect(ui->hand_Widget,&CTrainingHand::signalFingerStateChanged,this,&PassiveTraining::slotFingerStateChanged);
    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    //设置当前手的训练状态为被动
    ui->hand_Widget->setMode(TRAINMODE_PASSIVE_E);
    ui->rang_Dial->setValue(10);

    for(int i =0;i < 5;i++)
    {
        m_bucklingStrength[i] = 0;
        m_stretchStrength[i] = 0;
    }
    m_timeOutTimer = new QTimer();
    m_timeOutTimer->setSingleShot(true);

    connect(m_timeOutTimer,&QTimer::timeout,this,&PassiveTraining::slotProcessTimeOut);

    st_passiveParam.speed = 2;
    st_passiveParam.strength = 1;

    ui->times_RadioButton->setChecked(true);

    m_finishTimer = new QTimer();
    connect(m_finishTimer,&QTimer::timeout,this,&PassiveTraining::slotFinishTimer);
    m_finishTimer->setInterval(100);

    if(0 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(true);
    else if(1 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(false);

    ui->four_Widget->setAreaType(1);
    ui->thumb_Widget->setAreaType(0);

    ui->speed_Dial->setValue(1);

    ui->pause_Btn->setVisible(false);
}

PassiveTraining::~PassiveTraining()
{
    if(m_positionTimer)
    {
        delete  m_positionTimer;
        m_positionTimer = NULL;
    }
    if(m_timeOutTimer)
    {
        delete  m_timeOutTimer;
        m_timeOutTimer = NULL;
    }
    delete ui;
}

void PassiveTraining::initWidget()
{
    QString allFingers("./source/GIF/allFingers5.gif");
    QString fourToOne7("./source/GIF/fourToOne7.gif");
    QString leftToright13("./source/GIF/leftToright13.gif");
    QString rightToLeft13("./source/GIF/rightToLeft13.gif");

    ui->allFingers_Btn->setMovieFilePathAndFrames(allFingers,5);
    ui->fourToOne_Btn->setMovieFilePathAndFrames(fourToOne7,7);
    ui->leftToRight_Btn->setMovieFilePathAndFrames(leftToright13,13);
    ui->rightToLeft_Btn->setMovieFilePathAndFrames(rightToLeft13,13);

}

//所有手指同时动作
void PassiveTraining::on_allFingers_Btn_clicked()
{
    ui->fourToOne_Btn->setSelected(false);
    if(ui->allFingers_Btn->getSelectedState())
        m_actionType = -1;
    else
        m_actionType = 0;
}
//先四肢再拇指
void PassiveTraining::on_fourToOne_Btn_clicked()
{
    ui->allFingers_Btn->setSelected(false);
    if(ui->fourToOne_Btn->getSelectedState())
        m_actionType = 1;
    else
        m_actionType = 0;
}

//小指到拇指
void PassiveTraining::on_leftToRight_Btn_clicked()
{
    ui->rightToLeft_Btn->setSelected(false);
    if(ui->leftToRight_Btn->getSelectedState())
        m_fingerDirection = -1;
    else
        m_fingerDirection = 0;
}

//拇指到小指
void PassiveTraining::on_rightToLeft_Btn_clicked()
{
    ui->leftToRight_Btn->setSelected(false);
    if(ui->rightToLeft_Btn->getSelectedState())
        m_fingerDirection = 1;
    else
        m_fingerDirection = 0;
}

//设置被动训练手指
void PassiveTraining::setPassiveTrainFinger(const ST_HandTypeFingers &st_HandTypeFingers)
{
    uint16_t cmdId = SELECT_PASSIVE_TRANNING_FINGER;

    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(ST_HandTypeFingers));
    memcpy(array.data(),&st_HandTypeFingers,sizeof(ST_HandTypeFingers));

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//设置被动训练参数
void PassiveTraining::setPassiveTrainParam(const ST_PassiveParam  &st_PassiveParam)
{
    uint16_t cmdId = PASSIVE_TRANNING_MODE_PARAMETER;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(ST_PassiveParam));
    memcpy(array.data(),&st_PassiveParam,sizeof(ST_PassiveParam));

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//控制被动训练开关
void PassiveTraining::switchPassiveTrain(bool isStart)
{
    uint16_t cmdId;
    if(isStart)
        cmdId = START_PASSIVE_TRANNING_MODE;
    else
        cmdId = CLOSE_PASSIVE_TRANNING_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void PassiveTraining::on_switchBucklingFace_Btn_clicked()
{
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    bool isShow = false;
    if(ui->switchBucklingFace_Btn->styleSheet() == turnOn)
    {
        isShow = false;
        ui->switchBucklingFace_Btn->setStyleSheet(turnOff);
    }
    else if(ui->switchBucklingFace_Btn->styleSheet() == turnOff)
    {
        isShow = true;
        ui->switchBucklingFace_Btn->setStyleSheet(turnOn);
    }

    ui->face_GroupBox->setVisible(isShow);
}

void PassiveTraining::on_switchStretchFace_Btn_clicked()
{
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    bool isShow = false;
    if(ui->switchStretchFace_Btn->styleSheet() == turnOn)
    {
        isShow = false;
        ui->switchStretchFace_Btn->setStyleSheet(turnOff);
    }
    else if(ui->switchStretchFace_Btn->styleSheet() == turnOff)
    {
        isShow = true;
        ui->switchStretchFace_Btn->setStyleSheet(turnOn);
    }
    ui->face1_GroupBox->setVisible(isShow);
}

//各手指间的时间间隔
void PassiveTraining::on_interval_Dial_valueChanged(int value)
{
    m_fingerInterval = value*300 + 10;
}
//四指和拇指之间的时间延迟
void PassiveTraining::on_delay_SpinBox_valueChanged(int arg1)
{
    m_processDelay = arg1*1000;
}

//速度
void PassiveTraining::on_speed_Dial_valueChanged(int value)
{
    st_passiveParam.speed = value;
    //    setPassiveTrainParam(st_passiveParam);
    calculateTimeBySpeed(value,100);
}

//力量极限
void PassiveTraining::on_strength_Dial_valueChanged(int value)
{
    st_passiveParam.strength = value + 1;
    setPassiveTrainParam(st_passiveParam);
}

void PassiveTraining::showEvent(QShowEvent *event)
{
    ui->trainTips_Label->setText(tr("准备中..."));
    Q_UNUSED(event)

    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    //1.设置左右手
    ui->hand_Widget->switchHand(m_handType);
    //2.更改箭头显示位置
    if(0 == m_handType)
        ui->direction_Label->move(160,364);
    else if(1 == m_handType)
        ui->direction_Label->move(370,364);
    ui->direction_Label->setVisible(false);
    //告知下位机进入被动运动模式
    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_PASSIVE_E);
    //获取边界值
    st_bucklingPosition = CurrentUserData::getInstace()->getMinDestPosition();

    st_stretchPosition = CurrentUserData::getInstace()->getMaxDestPosition();
    //新增，调试右手拇指位置12.10
    updateFingerMoveScope(1);
    //获取手指状态
    for(int i =0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        //初始化手指的各个状态
        for(int i = 0;i < 5;i++)
            ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
    }
    //设置手指的活动基本范围
    map_baseMoveScope = CurrentUserData::getInstace()->getOppositeRange();
    //    qDebug()<<map_baseMoveScope;
    ui->hand_Widget->setBaseMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //设置手指活动实际范围
    ui->hand_Widget->setRealMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //实际活动范围
    map_realMoveScope = CurrentUserData::getInstace()->getOppositeRange();

    ui->times_Label->setText("0");
    ui->duration_Label->setText("0");
}

void PassiveTraining::on_start_Btn_clicked()
{
    if(ui->start_Btn->text() == tr("开始"))
    {
//        ui->start_Btn->setText("停止");
        ui->start_Btn->setEnabled(false);
        ui->start_Btn->setStyleSheet("QPushButton{background: #D3D3D3;border-radius: 8px;color:white;}");
        ui->trainTips_Label->setText(tr("训练中..."));
        MainWindowPageControl::getInstance()->updatePageRunningflag(PassiveTrainingPage_E,true);
        m_finishTimer->start();
        m_isTraining = true;

        //开始训练
        switchPassiveTrain(true);
        //开启实时位置
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimePositon(true);
        //开启实时力量
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimeStrength(true);
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimeStrength(true);

        startTime = QDateTime::currentDateTime();
        st_trainReport.startTime = startTime;
        st_trainReport.trainMode = TRAINMODE_PASSIVE_E;
        st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
        st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;

        m_totalTimes = 0;
        ui->times_Label->setText("0");
        ui->duration_Label->setText("0");
        //开启训练流程
        trainProcess();

    }
    else if(ui->start_Btn->text() == tr("停止"))
    {
        ui->start_Btn->setText(tr("开始"));
        ui->trainTips_Label->setText(tr("准备中..."));
        m_finishTimer->stop();
        m_isTraining = false;
        //停止训练
        switchPassiveTrain(false);
        //开启实时位置
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimePositon(false);
        //开启实时力量
        Sleep(100);
        CCommunicateAPI::getInstance()->getRealTimeStrength(false);
    }
}

void PassiveTraining::on_pause_Btn_clicked()
{
    if(ui->pause_Btn->text() == tr("暂停"))
    {
        m_isTraining = false;
        ui->pause_Btn->setText(tr("继续"));
    }
    else if(ui->pause_Btn->text() == tr("继续"))
    {
        m_isTraining = true;
        ui->pause_Btn->setText(tr("暂停"));
    }
}

void PassiveTraining::on_finish_Btn_clicked()
{
    ui->trainTips_Label->setText(tr("准备中..."));
    MainWindowPageControl::getInstance()->updatePageRunningflag(PassiveTrainingPage_E,false);
    m_finishTimer->stop();
    m_isTraining = false;
//    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    //结束被动训练
    switchPassiveTrain(false);
    Sleep(100);
    //关闭实时位置
    CCommunicateAPI::getInstance()->getRealTimePositon(false);
    //关闭实时力量
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimeStrength(false);

    m_timeOutTimer->stop();

    endTime = QDateTime::currentDateTime();
    st_trainReport.endTime  = endTime;
    st_trainReport.duration = startTime.secsTo(endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.markMsg = QString(tr("共进行%1次抓握练习")).arg(ui->times_Label->text());
    //将训练数据存入数据库
    QString table("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,trainReportToVariantMap(st_trainReport)))
    {
        qDebug()<<"被动训练结果存储失败";
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
    }

    m_totalTimes = 0;

    MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);

    clearWidget();

    ui->start_Btn->setEnabled(true);
    ui->start_Btn->setStyleSheet("QPushButton{background: #0D9DDB;border-radius: 8px;color:white;}\nQPushButton:hover{background: #1EAACF;border-radius: 8px;color:#333333;}");
}

void PassiveTraining::receiveCommData(QByteArray receiveData)
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
            st_handPosition.middlefinger = st_RealTimePosition.middlefinger;
            st_handPosition.ringfinger = st_RealTimePosition.ringfinger;
            st_handPosition.littlefinger = st_RealTimePosition.littlefinger;

            setHandFingersPosition(st_handPosition);

//            ui->currThumb_label->setText(QString::number(100-st_handPosition.thumb));
//            ui->currFore_Label->setText(QString::number(st_handPosition.forefinger));
//            ui->currMidd_Label->setText(QString::number(st_handPosition.middlefinger));
//            ui->currRing_Label->setText(QString::number(st_handPosition.ringfinger));
//            ui->currLittle_Label->setText(QString::number(st_handPosition.littlefinger));
        }
            break;
        case RESPONDFORCE://获取手指实时力量数据
        {
            ST_RealTimeStrength st_RealTimeStrength;
            memcpy(&st_RealTimeStrength,receiveData.data() + 6,sizeof(ST_RealTimeStrength));

//            ui->forceThumb_label->setText(QString::number(st_RealTimeStrength.thumb));
//            ui->forceFore_Label->setText(QString::number(st_RealTimeStrength.forefinger));
//            ui->forceMidd_Label->setText(QString::number(st_RealTimeStrength.middlefinger));
//            ui->forceRing_Label->setText(QString::number(st_RealTimeStrength.ringfinger));
//            ui->forceLittle_Label->setText(QString::number(st_RealTimeStrength.littlefinger));

            //绘制力量域图
            paintAreaChart(st_RealTimeStrength);
            //计算力量极值
            calculateBucklingStrength(st_RealTimeStrength);
            calculateStretchStrength(st_RealTimeStrength);
        }
            break;
        }
    }
}

//绘制力量域图
void PassiveTraining::paintAreaChart(ST_RealTimeStrength& st_RealTimeStrength)
{
    ui->four_Widget->repaintChart(st_RealTimeStrength);
    ui->thumb_Widget->repaintChart(st_RealTimeStrength.thumb);
}

//屈曲力打分取值0~100
void PassiveTraining::setBucklingScore(int strenth)
{
    if(strenth > 0 && strenth <=20)
    {
        ui->bucklingFace_Widget->setExpressionResult(4);
    }
    else if(strenth > 20 && strenth <= 40)
    {
        ui->bucklingFace_Widget->setExpressionResult(3);
    }
    else if(strenth > 40 && strenth <= 60)
    {
        ui->bucklingFace_Widget->setExpressionResult(2);
    }
    else if(strenth > 60 && strenth <= 80)
    {
        ui->bucklingFace_Widget->setExpressionResult(1);
    }
    else if(strenth > 80 && strenth <= 250)
    {
        ui->bucklingFace_Widget->setExpressionResult(0);
    }
}
//伸展力打分取值 0~100
void PassiveTraining::setStretchScore(int strenth)
{
    if(strenth > 0 && strenth <=20)
    {
        ui->stretchFace_Widget->setExpressionResult(4);
    }
    else if(strenth > 20 && strenth <= 40)
    {
        ui->stretchFace_Widget->setExpressionResult(3);
    }
    else if(strenth > 40 && strenth <= 60)
    {
        ui->stretchFace_Widget->setExpressionResult(2);
    }
    else if(strenth > 60 && strenth <= 80)
    {
        ui->stretchFace_Widget->setExpressionResult(1);
    }
    else if(strenth > 80 && strenth <= 250)
    {
        ui->stretchFace_Widget->setExpressionResult(0);
    }
}

//控制单个手指到目标位置
void PassiveTraining::setFingerMoveToDestPosition(int8_t fingerIndex,uint8_t position)
{

//    switch(fingerIndex)
//    {
//    case 0:
//        ui->destThumb_label->setText(QString::number(position));
//        break;
//    case 1:
//        ui->destFore_Label->setText(QString::number(position));
//        break;
//    case 2:
//        ui->destMidd_Label->setText(QString::number(position));
//        break;
//    case 3:
//        ui->destRing_Label->setText(QString::number(position));
//        break;
//    case 4:
//        ui->destLittle_Label->setText(QString::number(position));
//        break;
//    }


    ST_FingerDestPosition st_fingerDestPosition;
    st_fingerDestPosition.fingerIndex = fingerIndex;
    //    //右手拇指
    //    if((0 ==fingerIndex) && (1 == m_handType))
    //    {
    //        st_fingerDestPosition.position =100 - position;
    //    }
    //    else
    st_fingerDestPosition.position = position;
    uint16_t cmdId = PASSIVE_CONTROL_TO_DESTPOSITION;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(ST_FingerDestPosition));
    memcpy(array.data(),&st_fingerDestPosition,sizeof(ST_FingerDestPosition));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//手指状态发生变化
void PassiveTraining::slotFingerStateChanged(int8_t index,int8_t state)
{
    //更新当前手指状态
    fingersState[index] = originalSteate[index] & state;
    //更新手指状态
    for(int i = 0;i < 5;i++)
        ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
}

//训练流程
void PassiveTraining::trainProcess()
{
    if(m_isTraining)
    {
        if(st_passiveParam.speed!= m_lastSpeed)
        {
            m_lastSpeed = st_passiveParam.speed;
            setPassiveTrainParam(st_passiveParam);
        }
        static int times = 0;//用于拇指和四指分开的流程

        //训练流程（拇指和四指是否分开训练）
        switch(m_actionType)
        {
        case -1://五指同时
        {
            switch(m_fingerDirection)
            {
            case -1://从小指到拇指
            {
                fromLittleToThumb();
                m_timeOutTimer->start(timeOutTime);
            }
                break;
            case 1://从拇指到小指
            {
                formThumbToLittle();
                m_timeOutTimer->start(timeOutTime);
            }
                break;
            }
        }
            break;
        case 1://拇指和四指分开
        {
            //四指流程
            switch(m_fingerDirection)
            {
            case -1://从小指到食指
            {
                //拇指流程
                if(-1 == m_trainProcessType)
                {
                    times++;
                    thumbMoveProcess(m_moveDirection);
                    m_timeOutTimer->start(2500);
                    if(times >= 2)
                    {
                        times = 0;
                        m_trainProcessType = 1;
                    }
                }
                //四指流程
                else if(1 == m_trainProcessType)
                {
                    //拇指和四指间的时间间隔
                    Sleep(m_processDelay);
                    times++;
                    fromLittleToForefinger();
                    m_timeOutTimer->start(timeOutTime);
                    if(times >= 2)
                    {
                        times = 0;
                        m_trainProcessType = -1;
                    }
                }
            }
                break;
            case 1://从食指到小指
            {
                //拇指流程
                if(-1 == m_trainProcessType)
                {
                    times++;
                    thumbMoveProcess(m_moveDirection);
                    //拇指运行时间相对固定
                    m_timeOutTimer->start(2500);
                    if(times >= 2)
                    {
                        times = 0;
                        m_trainProcessType = 1;
                    }
                }
                //四指流程
                else if(1 == m_trainProcessType)
                {
                    Sleep(m_processDelay);
                    times++;
                    fromForefingerToLittle();
                    m_timeOutTimer->start(timeOutTime);
                    if(times >= 2)
                    {
                        times = 0;
                        m_trainProcessType = -1;
                    }
                }
            }
                break;
            }
        }
            break;
        }
        m_totalTimes++;
        ui->times_Label->setText(QString::number(m_totalTimes));
        QDateTime dateTime = QDateTime::currentDateTime();
        qint64 msecond = startTime.msecsTo(dateTime);
        ui->duration_Label->setText(QString::number(msecond/1000/60));

        //给出打分
        int scoreNum = 0;
        for(int i = 0;i < 5;i++)
        {
            scoreNum += fingersState[i];
        }
        //屈曲力量
        if(-1 == m_moveDirection)
        {
            int buckling = 0;
            for(int i = 0; i < 5;i++)
            {
                buckling += m_bucklingStrength[i];
            }
            if(scoreNum > 0)
                setBucklingScore(buckling/(float)scoreNum);
//            ui->bucklingScore_Label->setText(QString::number(buckling/(float)scoreNum));
        }
        //伸展力打分
        else if(1 == m_moveDirection)
        {
            int stretch = 0;
            for(int i =0;i < 5;i++)
            {
                stretch += m_stretchStrength[i];
            }
            if(scoreNum > 0)
                setStretchScore(stretch/(float)scoreNum);
//            ui->stretchScore_Label->setText(QString::number(stretch/(float)scoreNum));
        }
//        ui->score_Label->setText(QString::number(scoreNum));
        //力量清空
        for(int i =0;i < 5;i++)
        {
            m_bucklingStrength[i] = 0;
            m_stretchStrength[i] = 0;
        }
    }
}

//流程超时槽函数
void PassiveTraining::slotProcessTimeOut()
{
    if(m_isTraining)
    {
        m_moveDirection*=(-1);
        trainProcess();
    }
}

void PassiveTraining::setHandFingersPosition(ST_HandFingersPosition &st_FingerPosition)
{
    if(1 == m_handType)
        ui->hand_Widget->setHandFingerValue(m_handType,0,100 - st_FingerPosition.thumb);
    else if(0 == m_handType)
        ui->hand_Widget->setHandFingerValue(m_handType,0,st_FingerPosition.thumb);

    ui->hand_Widget->setHandFingerValue(m_handType,1,st_FingerPosition.forefinger);
    ui->hand_Widget->setHandFingerValue(m_handType,2,st_FingerPosition.middlefinger);
    ui->hand_Widget->setHandFingerValue(m_handType,3,st_FingerPosition.ringfinger);
    ui->hand_Widget->setHandFingerValue(m_handType,4,st_FingerPosition.littlefinger);
}

void PassiveTraining::on_verticalScrollBar_valueChanged(int value)
{
    //滑块所在的位置
    float percent = 0;
    if(0 != ui->verticalScrollBar->maximum()-ui->verticalScrollBar->minimum())
        percent = (float)value/(ui->verticalScrollBar->maximum()-ui->verticalScrollBar->minimum());
    else
        percent = 0.5;
    updateFingerMoveScope(percent);
}
//关节活动度
void PassiveTraining::on_rang_Dial_valueChanged(int value)
{
    m_scopePercent = value/10.f;
    value = 10 - value;
    ui->verticalScrollBar->setRange(0,value*2);
    ui->verticalScrollBar->setValue(value);
}

void PassiveTraining::updateFingerMoveScope(float upDownPercent)
{
    QMapIterator<int,QPair<int,int>> baseIter(map_baseMoveScope);
    QMapIterator<int,QPair<int,int>> realIter(map_realMoveScope);
    int i = 0;
    while(baseIter.hasNext() && realIter.hasNext())
    {
        baseIter.next();
        realIter.next();
        //基础活动范围
        float baseScope = qAbs(baseIter.value().second- baseIter.value().first);
        //新的活动范围
        float newScope = baseScope * m_scopePercent;
        //重新设定范围
        int newBaseMin = 0;
        if(0 == i)
            newBaseMin = baseIter.value().first + (baseScope-newScope)*(1-upDownPercent);
        else
            newBaseMin = baseIter.value().first + (baseScope-newScope)*upDownPercent;
        int newBaseMax = newBaseMin + newScope;
        //重新设定上下边界
        map_realMoveScope[i].first =  newBaseMin;
        map_realMoveScope[i].second = newBaseMax;
        //设置屈曲位置
        if(0 == i && 0 == m_handType)
        {
            st_bucklingPosition.array[i] = 100 - newBaseMin;
            st_stretchPosition.array[i] = 100 - newBaseMax;
        }
        else
        {
            st_bucklingPosition.array[i] = newBaseMax;
            st_stretchPosition.array[i] = newBaseMin;
        }
        i++;
    }

    syncdata(st_bucklingPosition);
    syncdata(st_stretchPosition);

    ui->hand_Widget->setRealMoveScope(map_realMoveScope);
}

//计算屈曲力量极值
void PassiveTraining::calculateBucklingStrength(const ST_RealTimeStrength& st_realStrength)
{

    if(m_bucklingStrength[0] < qAbs(st_realStrength.thumb))
        m_bucklingStrength[0] = qAbs(st_realStrength.thumb);
    if(m_bucklingStrength[1] < qAbs(st_realStrength.forefinger))
        m_bucklingStrength[1] = qAbs(st_realStrength.forefinger);
    if(m_bucklingStrength[2] < qAbs(st_realStrength.middlefinger))
        m_bucklingStrength[2] = qAbs(st_realStrength.middlefinger);
    if(m_bucklingStrength[3] < qAbs(st_realStrength.ringfinger))
        m_bucklingStrength[3] = qAbs(st_realStrength.ringfinger);
    if(m_bucklingStrength[4] < qAbs(st_realStrength.littlefinger))
        m_bucklingStrength[4] = qAbs(st_realStrength.littlefinger);
}
//计算伸展力量极值
void PassiveTraining::calculateStretchStrength(const ST_RealTimeStrength&st_realStrength)
{
    if(m_stretchStrength[0] < qAbs(st_realStrength.thumb))
        m_stretchStrength[0] = qAbs(st_realStrength.thumb);
    if(m_stretchStrength[1] < qAbs(st_realStrength.forefinger))
        m_stretchStrength[1] = qAbs(st_realStrength.forefinger);
    if(m_stretchStrength[2] < qAbs(st_realStrength.middlefinger))
        m_stretchStrength[2] = qAbs(st_realStrength.middlefinger);
    if(m_stretchStrength[3] < qAbs(st_realStrength.ringfinger))
        m_stretchStrength[3] = qAbs(st_realStrength.ringfinger);
    if(m_stretchStrength[4] < qAbs(st_realStrength.littlefinger))
        m_stretchStrength[4] = qAbs(st_realStrength.littlefinger);
}

void PassiveTraining::controlFiveFingersMove( ST_FingersPosition& st_fingerPositon)
{

    uint16_t cmdId = PASSIVE_CONTROL_ALL_FINGERS;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(ST_FingersPosition));
    memcpy(array.data(),&st_fingerPositon,sizeof(ST_FingersPosition));
    CCommunicateAPI::getInstance()->sendData(seq,id, cmdId,array);
}

/*********五指同时活动***********/
//从拇指到小指流程
void PassiveTraining::formThumbToLittle()
{
    //先判断手指间是否有延时
    if(/*0 &&*/ 10 == m_fingerInterval)//无延时
    {
        ST_FingersPosition st_fingerPositon;
        //根据目标方向确定各手指的目标位置
        switch(m_moveDirection)
        {
        case -1://运动到屈曲位置
        {
            if(fingersState[0])
                st_fingerPositon.thumb =st_bucklingPosition.thumb;
            else
                st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_bucklingPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_bucklingPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_bucklingPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_bucklingPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        case 1://运动到伸展位置
        {
            if(fingersState[0])
                st_fingerPositon.thumb =st_stretchPosition.thumb;
            else
                st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_stretchPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_stretchPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_stretchPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_stretchPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        }
        //告知下位机同时运动手指
        controlFiveFingersMove(st_fingerPositon);
    }
    else//有延时
    {
        ST_DestPosition st_destPosition;
        //根据目标方向来给目标位置赋值
        switch(m_moveDirection)
        {
        case -1:
            st_destPosition = st_bucklingPosition;
            break;
        case 1:
            st_destPosition = st_stretchPosition;
            break;
        }
        for(int i = 0;i < 5;i++)
        {
            //只有被选中的手指才参与训练
            if(fingersState[i])
            {
                setFingerMoveToDestPosition(i,st_destPosition.array[i]);
                Sleep(m_fingerInterval);
            }
        }
    }
}
//从小指到拇指流程
void PassiveTraining::fromLittleToThumb()
{
    //先判断手指间是否有延时
    if(/*0 &&*/ 10 == m_fingerInterval)//无延时
    {
        ST_FingersPosition st_fingerPositon;
        //根据目标方向确定各手指的目标位置
        switch(m_moveDirection)
        {
        case -1://运动到屈曲位置
        {
            if(fingersState[0])
                st_fingerPositon.thumb =st_bucklingPosition.thumb;
            else
                st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_bucklingPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_bucklingPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_bucklingPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_bucklingPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        case 1://运动到伸展位置
        {
            if(fingersState[0])
                st_fingerPositon.thumb =st_stretchPosition.thumb;
            else
                st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_stretchPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_stretchPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_stretchPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_stretchPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        }
        //告知下位机同时运动手指
        controlFiveFingersMove(st_fingerPositon);
    }
    else//有延时
    {
        ST_DestPosition st_destPosition;
        //根据目标方向来给目标位置赋值
        switch(m_moveDirection)
        {
        case -1:
            st_destPosition = st_bucklingPosition;
            break;
        case 1:
            st_destPosition = st_stretchPosition;
            break;
        }
        for(int i = 4;i >=0 ;i--)
        {
            //只有被选中的手指才参与训练
            if(fingersState[i])
            {
                setFingerMoveToDestPosition(i,st_destPosition.array[i]);
                Sleep(m_fingerInterval);
            }
        }
    }
}
/*********四指拇指分开活动*******/
//从小指到食指
void PassiveTraining::fromLittleToForefinger()
{
    //先判断手指间是否有延时
    if(10 == m_fingerInterval)//无延时
    {
        ST_FingersPosition st_fingerPositon;
        //根据目标方向确定各手指的目标位置
        switch(m_moveDirection)
        {
        case -1://运动到屈曲位置
        {
            //保持拇指不动
            st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_bucklingPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_bucklingPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_bucklingPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_bucklingPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        case 1://运动到伸展位置
        {
            //保持拇指不动
            st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_stretchPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_stretchPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_stretchPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_stretchPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        }
        //告知下位机同时运动手指
        controlFiveFingersMove(st_fingerPositon);
    }
    else//有延时
    {
        ST_DestPosition st_destPosition;
        //根据目标方向来给目标位置赋值
        switch(m_moveDirection)
        {
        case -1:
            st_destPosition = st_bucklingPosition;
            break;
        case 1:
            st_destPosition = st_stretchPosition;
            break;
        }
        //从食指开始
        for(int i = 4;i >0 ;i--)
        {
            //只有被选中的手指才参与训练
            if(fingersState[i])
            {
                setFingerMoveToDestPosition(i,st_destPosition.array[i]);
                Sleep(m_fingerInterval);
            }
        }
    }
}

//从食指到小指
void PassiveTraining::fromForefingerToLittle()
{
    //先判断手指间是否有延时
    if(10 == m_fingerInterval)//无延时
    {
        ST_FingersPosition st_fingerPositon;
        //根据目标方向确定各手指的目标位置
        switch(m_moveDirection)
        {
        case -1://运动到屈曲位置
        {
            //保持拇指不动
            st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_bucklingPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_bucklingPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_bucklingPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_bucklingPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        case 1://运动到伸展位置
        {
            //保持拇指不动
            st_fingerPositon.thumb = -1;
            if(fingersState[1])
                st_fingerPositon.forefinger =st_stretchPosition.forefinger;
            else
                st_fingerPositon.forefinger = -1;
            if(fingersState[2])
                st_fingerPositon.middlefinger =st_stretchPosition.middlefinger;
            else
                st_fingerPositon.middlefinger = -1;
            if(fingersState[3])
                st_fingerPositon.ringfinger =st_stretchPosition.ringfinger;
            else
                st_fingerPositon.ringfinger = -1;
            if(fingersState[4])
                st_fingerPositon.littlefinger =st_stretchPosition.littlefinger;
            else
                st_fingerPositon.littlefinger = -1;
        }
            break;
        }
        //告知下位机同时运动手指
        controlFiveFingersMove(st_fingerPositon);
    }
    else//有延时
    {
        ST_DestPosition st_destPosition;
        //根据目标方向来给目标位置赋值
        switch(m_moveDirection)
        {
        case -1:
            st_destPosition = st_bucklingPosition;
            break;
        case 1:
            st_destPosition = st_stretchPosition;
            break;
        }
        //从食指开始
        for(int i = 1;i < 5;i++)
        {
            //只有被选中的手指才参与训练
            if(fingersState[i])
            {
                setFingerMoveToDestPosition(i,st_destPosition.array[i]);
                Sleep(m_fingerInterval);
            }
        }
    }
}

//运动拇指
void PassiveTraining::thumbMoveProcess(int8_t direction)
{
    switch(direction)
    {
    case -1://当前伸展-屈曲-伸展
        setFingerMoveToDestPosition(0,st_bucklingPosition.thumb);
        Sleep(1000);
        setFingerMoveToDestPosition(0,st_stretchPosition.thumb);
        break;
    case 1://当前屈曲-伸展-屈曲
        setFingerMoveToDestPosition(0,st_stretchPosition.thumb);
        Sleep(1000);
        setFingerMoveToDestPosition(0,st_bucklingPosition.thumb);
        break;
    }
}

//根据速度以及距离计算时间
void PassiveTraining::calculateTimeBySpeed(int speed,int length)
{
    float realSpeed = 0;
    switch(speed)
    {
    case 1:
        realSpeed = 0.0095;
        break;
    case 2:
        realSpeed = 100/6650.f; //0.015
        break;
    case 3:
        realSpeed = 100/3750.f; //0.026
        break;
    case 4:
        realSpeed = 100/3000.f; //0.0408
        break;
    case 5:
        realSpeed = 100/2450.f; //0.0645
        break;
    }
    if(realSpeed != 0)
        timeOutTime = length/realSpeed;
}



void PassiveTraining::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == PassiveTrainingPage_E)
    {
        on_finish_Btn_clicked();
    }
}

//设定流程是否完成
void PassiveTraining::slotFinishTimer()
{
    //根据次数进行判断
    if(ui->times_RadioButton->isChecked())
    {
        if(ui->times_Label->text().toInt() > ui->times_SpinBox->value())
        {
            QMessageBox::warning(NULL,tr("提示"),tr("达到设定抓握次数，训练停止"));
            on_finish_Btn_clicked();
        }
    }
    //根据持续时间判断
    else if(ui->duration_RadioButton->isChecked())
    {
        if(ui->duration_Label->text().toInt() > ui->duration_SpinBox->value())
        {
            QMessageBox::warning(NULL,tr("提示"),tr("持续时间到达设定总时长，训练停止"));
            on_finish_Btn_clicked();
        }
    }
}

void PassiveTraining::slotCurrentUserChanged()
{
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    //1.设置左右手
    ui->hand_Widget->switchHand(m_handType);
    //2.更改箭头显示位置
    if(0 == m_handType)
        ui->direction_Label->move(160,364);
    else if(1 == m_handType)
        ui->direction_Label->move(370,364);
    ui->direction_Label->setVisible(false);

    st_bucklingPosition = CurrentUserData::getInstace()->getMinDestPosition();
    st_stretchPosition = CurrentUserData::getInstace()->getMaxDestPosition();

    //获取手指状态
    for(int i =0;i < 5;i++)
    {
        originalSteate[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        fingersState[i] = CurrentUserData::getInstace()->getCurrentUserHandData().fingerSelected[i];
        //初始化手指的各个状态
        for(int i = 0;i < 5;i++)
            ui->hand_Widget->setHandFingerEnable(m_handType,i,fingersState[i]);
    }
    //设置手指的活动基本范围
    map_baseMoveScope = CurrentUserData::getInstace()->getOppositeRange();
    ui->hand_Widget->setBaseMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //设置手指活动实际范围
    ui->hand_Widget->setRealMoveScope(CurrentUserData::getInstace()->getOppositeRange());
    //实际活动范围
    map_realMoveScope = CurrentUserData::getInstace()->getOppositeRange();

    ui->times_Label->setText("0");
    ui->duration_Label->setText("0");
}

void PassiveTraining::setTestVisible(bool isShow)
{
//    ui->test_GroupBox->setVisible(isShow);
}

void PassiveTraining::clearWidget()
{
    //此处清除域图会导致软件崩溃
    ui->four_Widget->resetAreatChart();
    ui->thumb_Widget->resetAreatChart();
    ui->times_Label->setText("0");
    ui->duration_Label->setText("0");
    ui->delay_SpinBox->setValue(1);
    ui->speed_Dial->setValue(1);
    ui->rang_Dial->setValue(10);
    ui->strength_Dial->setValue(0);
    ui->interval_Dial->setValue(0);
    m_totalTimes = 0;
}

void PassiveTraining::syncdata(ST_DestPosition& destPosition)
{
    destPosition.thumb = destPosition.array[0];
    destPosition.forefinger = destPosition.array[1];
    destPosition.middlefinger = destPosition.array[2];
    destPosition.ringfinger = destPosition.array[3];
    destPosition.littlefinger = destPosition.array[4];

}
