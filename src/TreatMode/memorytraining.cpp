#include "memorytraining.h"
#include "ui_memorytraining.h"
#include <QMessageBox>
#include "currentuserdata.h"
#include "ccommunicateapi.h"
#include <QDebug>
#include <QTimer>
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include "readconfig.h"
#define BASEFORCE 50
MemoryTraining::MemoryTraining(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryTraining),
    m_handType(0),
    m_controlSet(NULL),
    m_isTrainning(false),
    m_currentMode(1),    //默认力量模式
    m_currentDirection(-1),//屈曲向下
    m_timeOutTimer(NULL),
    m_forceTimer(NULL),
    m_positionTimer(NULL),
    m_theRestTime(10000),
    currentTime(0),
    m_isLoop(false),
    m_lastProcessNum(0),
    m_currentProcessTrainFingerNum(1),
    m_currentProcessIndex(0),
    m_visualTips(false),    //视觉提示
    m_moveTips(false)       //运动提示
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

    connect(m_timeOutTimer,&QTimer::timeout,this,&MemoryTraining::slotTimerOut);
    connect(m_forceTimer,&QTimer::timeout,this,&MemoryTraining::slotChargeForceDirection);
    connect(m_positionTimer,&QTimer::timeout,this,&MemoryTraining::slotChargePosition);
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));
    connect(CurrentUserData::getInstace(),SIGNAL(signalTrainUserChanged()),this,SLOT(slotCurrentUserChanged()));

    for(int i =0;i < 5;i++)
        fingerEnable[i] = 1;

    ui->timeOut_ProgressBar->setValue(100);

    if(0 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(true);
    else if(1 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(false);

}

MemoryTraining::~MemoryTraining()
{
    lableList.clear();

    if(m_controlSet)
    {
        delete  m_controlSet;
        m_controlSet = NULL;
    }
    delete ui;
}

void MemoryTraining::initWidget()
{
    bucklingTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeNormal.png);}\n"
                                      "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeHover.png);}");
    bucklingTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/bucklingTypeSelected.png);}");

    stretchTypeStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeNormal.png);}\n"
                                     "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeHover.png);}");
    stretchTypeStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/stretchTypeSelected.png);}");

    circleLockStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/memoryNormal.png);}\n"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/activeTraining/memoryHover.png);}");
    circleLockStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/activeTraining/memorySelect.png);}");

    ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.second);

    for(int i = 0;i < 3;i++)
    {
        QString selected = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1Selected.png);}").arg(i+1);;
        levelSelectedList.append(selected);
        QString normal = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1NoSelect.png);}\n"
                                 "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/level%2Hover.png);}").arg(i+1).arg(i+1);
        levelNormalList.append(normal);
    }
    ui->level1_Btn->setStyleSheet(levelSelectedList.at(0));

    foreach(QObject* object,ui->sequence_GroupBox->children())
    {
        if(object->objectName().contains("mlabel"))
            lableList.append(static_cast<QLabel*>(object));
    }
}

void MemoryTraining::on_bucklingType_Btn_clicked()
{
    if(ui->bucklingType_Btn->styleSheet() == bucklingTypeStyle.first)
    {
        ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.second);
        ui->stretchType_Btn->setStyleSheet(stretchTypeStyle.first);
    }
    m_currentDirection = -1;
}

void MemoryTraining::on_stretchType_Btn_clicked()
{
    if(ui->stretchType_Btn->styleSheet() == stretchTypeStyle.first)
    {
        ui->stretchType_Btn->setStyleSheet(stretchTypeStyle.second);
        ui->bucklingType_Btn->setStyleSheet(bucklingTypeStyle.first);
    }
    m_currentDirection = 1;
}

void MemoryTraining::on_cycle_Btn_clicked()
{
//    if(ui->cycle_Btn->styleSheet() == circleLockStyle.first)
//    {
//        m_isLoop = true;
//        ui->cycle_Btn->setStyleSheet(circleLockStyle.second);
//    }
//    else if(ui->cycle_Btn->styleSheet() == circleLockStyle.second)
//    {
//        m_isLoop = false;
//        ui->cycle_Btn->setStyleSheet(circleLockStyle.first);
//    }
}

void MemoryTraining::on_level1_Btn_clicked()
{
    if(ui->level1_Btn->styleSheet() == levelNormalList.at(0))
    {
        ui->level1_Btn->setStyleSheet(levelSelectedList.at(0));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
    sendTrainLevel(1);
}

void MemoryTraining::on_level2_Btn_clicked()
{
    if(ui->level2_Btn->styleSheet() == levelNormalList.at(1))
    {
        ui->level2_Btn->setStyleSheet(levelSelectedList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
        ui->level3_Btn->setStyleSheet(levelNormalList.at(2));
    }
    sendTrainLevel(2);
}

void MemoryTraining::on_level3_Btn_clicked()
{
    if(ui->level3_Btn->styleSheet() == levelNormalList.at(2))
    {
        ui->level3_Btn->setStyleSheet(levelSelectedList.at(2));
        ui->level2_Btn->setStyleSheet(levelNormalList.at(1));
        ui->level1_Btn->setStyleSheet(levelNormalList.at(0));
    }
    sendTrainLevel(3);
}

void MemoryTraining::on_visual_Btn_clicked()
{
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    if(ui->visual_Btn->styleSheet() == turnOff)
    {
        m_visualTips = true;
        ui->visual_Btn->setStyleSheet(turnOn);
    }
    else if(ui->visual_Btn->styleSheet() == turnOn)
    {
        m_visualTips = false;
        ui->visual_Btn->setStyleSheet(turnOff);
    }
}

void MemoryTraining::on_moveTip_Btn_clicked()
{
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    if(ui->moveTip_Btn->styleSheet() == turnOff)
    {
        m_moveTips = true;
        ui->moveTip_Btn->setStyleSheet(turnOn);
    }
    else if(ui->moveTip_Btn->styleSheet() == turnOn)
    {
        m_moveTips = false;
        ui->moveTip_Btn->setStyleSheet(turnOff);
    }
}

void MemoryTraining::on_controlSet_Btn_clicked()
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

void MemoryTraining::on_start_Btn_clicked()
{
    ui->start_Btn->setEnabled(false);
    ui->start_Btn->setStyleSheet("QPushButton{background: #D3D3D3;border-radius: 8px;color:white;}");
    ui->trainTips_Label->setText(tr("训练中..."));
    MainWindowPageControl::getInstance()->updatePageRunningflag(MemoryTrainingPage_E,true);

    ui->timeOut_ProgressBar->setValue(100);

    m_isTrainning = true;
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));

    //将手指延时显示打开
    ui->hand_Widget->switchFingerDelayShow(true);

    ST_ControlParam st_ControlParam = m_controlSet->getControlParam();
    m_currentMode = st_ControlParam.controlType;
    //设置训练模式
    switchControlMode(st_ControlParam.controlType);

    //开始训练
    Sleep(100);
    switchMemoryTrain(true);

    //产生待训练的随机数
    m_randomList.clear();
    m_randomList.append(generateRandomNum(20));

    //位置和力量采集开关
    Sleep(100);
    if(2 == m_currentMode)
        CCommunicateAPI::getInstance()->getRealTimePositon(true);
    //打开实时力量开关
    else if(1 == m_currentMode)
        CCommunicateAPI::getInstance()->getRealTimeStrength(true);

    startTime = QDateTime::currentDateTime();
    st_trainReport.startTime = startTime;
    st_trainReport.trainMode = TRAINMODE_MEMORY_E;
    st_trainReport.ID = CurrentUserData::getInstace()->getCurrentUserID();
    st_trainReport.name = CurrentUserData::getInstace()->getCurrentPatientMsg().name;

    //进入训练流程
    Sleep(500);
    memeoryTrainProcess();
}

void MemoryTraining::on_stop_Btn_clicked()
{
    ui->trainTips_Label->setText(tr("准备中..."));
    MainWindowPageControl::getInstance()->updatePageRunningflag(MemoryTrainingPage_E,false);
    m_isTrainning = false;
    CCommunicateAPI::getInstance()->getRealTimePositon(false);
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimeStrength(false);
    Sleep(100);
    switchMemoryTrain(false);
    disconnect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));

    endTime = QDateTime::currentDateTime();
    st_trainReport.endTime  = endTime;
    st_trainReport.duration = startTime.secsTo(endTime)/60;
    if(st_trainReport.duration < 1)
        st_trainReport.duration = 1;
    st_trainReport.markMsg = QString(tr("记忆到第%1个魔方")).arg(m_lastProcessNum);
    //将训练数据存入数据库
    QString table("traintable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,trainReportToVariantMap(st_trainReport)))
    {
        qDebug()<<"记忆训练结果存储失败";
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
    }
    MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);

    ui->start_Btn->setEnabled(true);
    ui->start_Btn->setStyleSheet("QPushButton{background: #0D9DDB;border-radius: 8px;color:white;}\nQPushButton:hover{background: #1EAACF;border-radius: 8px;color:#333333;}");
}

void MemoryTraining::showEvent(QShowEvent *event)
{
    ui->trainTips_Label->setText(tr("准备中..."));
    Q_UNUSED(event)
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->setHandType(m_handType);

    //关节鼠标点击事件(在开始训练并且为点击状态下才开启)
    ui->hand_Widget->switchPressedEvent(false);

    //告知下位机进入主动运动模式
    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_MEMORY_E);
    //获取边界值
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

void MemoryTraining::receiveCommData(QByteArray receiveData)
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
    }
}

void MemoryTraining::switchMemoryTrain(bool isStart)
{
    uint16_t cmdId;
    if(isStart)
        cmdId = START_MEMORY_TRANNING_MODE;
    else
        cmdId = CLOSE_MEMORY_TRANNING_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MemoryTraining::switchControlMode(int8_t mode)
{
    uint16_t cmdId = MEMORY_MODE_CTRL_MODE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&mode,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//使当前手指
void MemoryTraining::rebackToOriginalPosition()
{
    uint16_t cmdId = MEMORY_MODE_REACH_TO_DEST;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&m_currentFingerIndex,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MemoryTraining::moveTipsCmd(int index,int8_t direction)
{
    uint16_t cmdId = MEMORY_MODE_MOVE_TIPS;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    ST_FingerUpDownControl st_fingerUpDownControl;
    st_fingerUpDownControl.fingerIndex = index;
    st_fingerUpDownControl.direction = direction;

    m_currentDirection = st_fingerUpDownControl.direction;
    array.resize(sizeof(ST_FingerUpDownControl));
    memcpy(array.data(),&st_fingerUpDownControl,sizeof(st_fingerUpDownControl));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

void MemoryTraining::sendTrainLevel(int8_t level)
{
    uint16_t cmdId = MEMORY_MODE_HELP_TYPE;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(sizeof(int8_t));
    memcpy(array.data(),&level,sizeof(int8_t));
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//控制矩阵显示颜色
void MemoryTraining::setMatrixColor(int index,E_Color color)
{
    QString greyColor("background: #3E3E3E;border: 1px solid #979797;");
    QString blueColor("background: #70C9F0;border: 1px solid #979797;");
    QString greenColor("background: #70E086;border: 1px solid #979797;");
    QString redColor("background: #FF767F;border: 1px solid #979797;");
    if(index >19)
        return;
    switch(color)
    {
    case Grey_E:
        lableList[index]->setStyleSheet(greyColor);
        break;
    case Green_E:
        lableList[index]->setStyleSheet(greenColor);
        break;
    case Red_E:
        lableList[index]->setStyleSheet(redColor);
        break;
    case Blue_E:
        lableList[index]->setStyleSheet(blueColor);
        break;
    }
}
//生成num个随机数
QList<uint8_t> MemoryTraining::generateRandomNum(int num)
{
    QList<uint8_t> list;
    while(list.size() < num)
    {
        qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        int randNum = qrand()%5;
        if(fingerEnable[randNum] == 0)
            continue;
        list.append(randNum);
        Sleep(10);
    }
    return list;
}

void MemoryTraining::on_pushButton_clicked()
{

    //    int index = ui->index_lineEdit->text().toUInt();
    //    int color = ui->color_lineEdit->text().toUInt();
    //    setMatrixColor(index,(E_Color)color);
    memeoryTrainProcess();
}

//倒计时计时函数
void MemoryTraining::slotTimerOut()
{
    currentTime+=50;
    ui->timeOut_ProgressBar->setValue((float)currentTime/m_theRestTime*100);
    //超时判断,超时--报错结束当前流程
    if(currentTime > m_theRestTime)
    {
        //给出手指错误提示
        ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateYellow_E);
        //给出矩阵错误提示
        stopCurrentProcess(false);
        qDebug()<<"超时警告";
        currentTime++;
    }
}


//运动提示测试
void MemoryTraining::on_moveTips_Btn_clicked()
{
//    m_currentFingerIndex = ui->fingerIndex_LineEdit->text().toUInt();
    moveTipsCmd(m_currentFingerIndex,m_currentDirection);
}

void MemoryTraining::on_goback_Btn_clicked()
{
    rebackToOriginalPosition();
}

//记忆训练流程
void MemoryTraining::memeoryTrainProcess()
{
    //重置显示矩阵
    resetMatrix();
    //首先清空当前流程带训练手指
    m_currentProcessToTrainFinger.clear();
    //根据手指数重新给待训练手指赋值
    for(int i = 0;i < m_currentProcessTrainFingerNum;i++)
        m_currentProcessToTrainFinger.append(m_randomList.at(i));
    qDebug()<<"此次需要训练的手指"<<m_currentProcessToTrainFinger;
    ui->seqLength_Label->setText(QString::number(m_currentProcessToTrainFinger.size()));
    //根据待训练手指给出运动提示
    giveTotalMoveTips(m_currentProcessToTrainFinger);
    //设置当前手指
    if(!m_currentProcessToTrainFinger.isEmpty())
        m_currentFingerIndex = m_currentProcessToTrainFinger.at(0);
    //给出视觉提示
    if(m_visualTips)
    {
        ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateGreen_E);
    }
    //给出运动提示
    if(m_moveTips)
    {
        moveTipsCmd(m_currentFingerIndex,m_currentDirection);
    }
    //打开倒计时定时器
    m_timeOutTimer->start();
    //（如果是力量模式）打开力量检测定时器
    if(1 == m_currentMode)
        m_forceTimer->start();
    //(如果是位置模式)打开位置检测定时器
    if(2 == m_currentMode)
        m_positionTimer->start();

    //运动前给出矩阵提示
    setMatrixColor(m_currentProcessIndex,Blue_E);
}
//根据待训练手指给出总提示
void MemoryTraining::giveTotalMoveTips(const QList<uint8_t>& fingerMoveTips)
{
    for(int i = 0;i < fingerMoveTips.size();i++)
    {
        moveTipsCmd(fingerMoveTips.at(i),m_currentDirection);
        Sleep(1000);
    }
}

//结束当前流程
void MemoryTraining::stopCurrentProcess(bool isRight)
{
    //记录本次训练中记忆的最大数量
    m_lastProcessNum = m_currentProcessTrainFingerNum;
    //关闭所有定时器
    m_timeOutTimer->stop();
    m_forceTimer->stop();
    m_positionTimer->stop();
    ui->timeOut_ProgressBar->setValue(0);
    //如果是位置模式则回到原点
    if(2 == m_currentMode)
        rebackToOriginalPosition();
    //数据全部清零
    if(!isRight)
        m_currentProcessTrainFingerNum = 1;
    m_currentProcessIndex = 0;
    m_currentProcessToTrainFinger.clear();
    currentTime = 0;
}

//力量模式下判断运动是否正确
void MemoryTraining::slotChargeForceDirection()
{
    //首先判断出超过力量设定值得手指
    int force[5] = {0};
    force[0] = st_currentFingerStrength.thumb;
    force[1] = st_currentFingerStrength.forefinger;
    force[2] = st_currentFingerStrength.middlefinger;
    force[3] = st_currentFingerStrength.ringfinger;
    force[4] = st_currentFingerStrength.littlefinger;

    int tempIndex = -1;//符合力量的下标
    for(int i = 0;i < 5;i++)
    {
        if(qAbs(force[i]) > BASEFORCE)
        {
            tempIndex = i;
            //根据下标判断是否是当前提示的手指以及方向是否正确
            if(tempIndex == m_currentFingerIndex)
            {
                switch(m_currentDirection)
                {
                case -1: //-1-屈曲（向下 力量为正直）
                {
                    if(force[tempIndex] > 0)
                    {
                        //给出正确提示反馈
                        setMatrixColor(m_currentProcessIndex,Green_E);
                        m_currentProcessIndex++;
                        setMatrixColor(m_currentProcessIndex,Blue_E);
                        //所有手指训练完成
                        if(m_currentProcessIndex >= m_currentProcessTrainFingerNum)
                        {
                            m_currentProcessTrainFingerNum++;
                            if(m_currentProcessTrainFingerNum >20)
                            {
                                //关闭所有流程(相当于从头开始)
                                stopCurrentProcess(false);
                            }
                            else
                            {
                                //将当前流程保持一秒
                                Sleep(1000);
                                //结束当前流程(正确结束)
                                stopCurrentProcess(true);
                                //开启新的流程
                                memeoryTrainProcess();
                            }
                        }
                        else //继续当前流程，下一个手指
                        {
                            m_currentFingerIndex = m_currentProcessToTrainFinger.at(m_currentProcessIndex);
                            //给出视觉提示
                            if(m_visualTips)
                            {
                                ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateGreen_E);
                            }
                            //给出运动提示
                            if(m_moveTips)
                            {
                                moveTipsCmd(m_currentFingerIndex,m_currentDirection);
                            }
                        }
                        resetTimeOutTimer();
                        Sleep(1000);
                    }
                    else
                    {
                        stopCurrentProcess(false);
                        qDebug()<<"ERROR屈曲移动错误,终止流程";
                        setMatrixColor(m_currentProcessIndex,Red_E);

                        //如果开启循环，则训练错误之后则继续训练
                        if(m_isLoop)
                        {
                            Sleep(1000);
                            memeoryTrainProcess();
                        }

                    }
                }
                    break;
                case 1://当前运动方向 1-伸展（向上 力量为负值）
                {
                    if(force[tempIndex] < 0)
                    {
                        qDebug()<<"伸展运动正确,继续流程";
                        //给出正确提示反馈
                        setMatrixColor(m_currentProcessIndex,Green_E);
                        m_currentProcessIndex++;
                        setMatrixColor(m_currentProcessIndex,Blue_E);

                        //所有手指训练完成
                        if(m_currentProcessIndex >= m_currentProcessTrainFingerNum)
                        {
                            m_currentProcessTrainFingerNum++;
                            if(m_currentProcessTrainFingerNum >20)
                            {
                                //关闭所有流程
                                stopCurrentProcess(false);
                            }
                            else
                            {
                                //结束当前流程
                                stopCurrentProcess(true);
                                //开启新的流程
                                memeoryTrainProcess();
                            }
                        }
                        else //继续当前流程，下一个手指
                        {
                            //继续当前流程
                            m_currentFingerIndex = m_currentProcessToTrainFinger.at(m_currentProcessIndex);
                            //给出视觉提示
                            if(m_visualTips)
                            {
                                ui->hand_Widget->setFingerState(m_currentFingerIndex,EvaluateGreen_E);
                            }
                            //给出运动提示
                            if(m_moveTips)
                            {
                                moveTipsCmd(m_currentFingerIndex,m_currentDirection);
                            }
                        }
                        resetTimeOutTimer();
                        Sleep(1000);
                    }
                    else
                    {
                        stopCurrentProcess(false);
                        setMatrixColor(m_currentProcessIndex,Red_E);

                        //如果开启循环，则训练错误之后则继续训练
                        if(m_isLoop)
                        {
                            Sleep(1000);
                            memeoryTrainProcess();
                        }
                    }
                }
                    break;
                }
            }
            else
            {
                setMatrixColor(m_currentProcessIndex,Red_E);
                stopCurrentProcess(false);

                //如果开启循环，则训练错误之后则继续训练
                if(m_isLoop)
                {
                    Sleep(1000);
                    memeoryTrainProcess();
                }
            }
            break;
        }
    }
}
//位置模式下判断是否正确
void MemoryTraining::slotChargePosition()
{
    m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
    ui->hand_Widget->setHandType(m_handType);
}

//重置倒计时
void MemoryTraining::resetTimeOutTimer()
{
    ui->timeOut_ProgressBar->setValue(0);
    currentTime = 0;
}

//重置显示矩阵
void MemoryTraining::resetMatrix()
{
    for(int i = 0;i < 20;i++)
        setMatrixColor(i,Grey_E);
}

void MemoryTraining::on_test_Btn_clicked()
{
    generateRandomNum(20);
}

void MemoryTraining::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == MemoryTrainingPage_E)
    {
        on_stop_Btn_clicked();
    }
}

void MemoryTraining::slotCurrentUserChanged()
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

//更新手指的状态
void MemoryTraining::updateFingersEnableState(int8_t* enableStates)
{
    for(int i = 0;i < 5;i++)
    {
        ui->hand_Widget->setFingerEnabled(i,enableStates[i]);
    }
}

void MemoryTraining::setTestVisible(bool isShow)
{
//    ui->test_GroupBox->setVisible(isShow);
}
