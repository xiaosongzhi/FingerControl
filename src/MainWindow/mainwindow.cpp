#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QButtonGroup>
#include "mainwindowpagecontrol.h"
#include "dataFormate.h"
//#include "controlmaskdialog.h"
#include "ui_mainwindow.h"
//#include "userfingerselect.h"
#include "currentuserdata.h"
#include <QDebug>
#include <QMessageBox>
#include "cpassworddialog.h"
#include "ccommunicateapi.h"
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_buttonGroup(NULL)
    , m_userManager(NULL)
    , m_heartBeatTimer(NULL)
    , m_isInEmergency(false)
    , m_disconnectDialog(NULL)
    , m_heartbeatLostTimes(0)
    , m_connectState(true)
    , m_lastConnectState(true)
    , m_displayWindow(NULL)
    , m_checkButtonGroup(NULL)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("64_64.ico"));
    setAttribute(Qt::WA_QuitOnClose,true);
    setWindowFlags(Qt::CustomizeWindowHint| Qt::FramelessWindowHint);

    //    setWindowState(Qt::WindowFullScreen);
    showFullScreen();
    //    resize(1920,1080);
    initWidgetUI();
    connect(CurrentUserData::getInstace(),SIGNAL(signalUserChanged()),this,SLOT(slotCurrentUserChanged()));
    MainWindowPageControl* pageControl = MainWindowPageControl::getInstance();
    connect(pageControl,SIGNAL(signalSwitchPage(int)),this,SLOT(slotSwitchCurrentPage(int)));
    connect(pageControl,SIGNAL(signalSetBtnsLock(bool)),this,SLOT(slotBtnsLock(bool)));
    setBtnsLcok(true);
    if(NULL == m_userManager)
    {
        m_userManager = new UserManager();
    }
    MainWindowPageControl::getInstance()->setCurrentPage(SettingPage_E);
    slotBtnGroupClicked(0);

    m_heartBeatTimer = new QTimer();
    connect(m_heartBeatTimer,&QTimer::timeout,this,&MainWindow::slotHeartBeatSlot);
    m_heartBeatTimer->start(500);
    this->statusBar()->setVisible(false);

    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(slotReceiveData(QByteArray)));
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalSendData(QByteArray)),this,SLOT(slotWatchSend(QByteArray)));

    m_disconnectDialog = new  WarningDialog();
    m_disconnectDialog->setWarningType(CONNECTLOST_E);

    m_displayWindow = new DisplayWindow();
}

MainWindow::~MainWindow()
{
    if(m_displayWindow)
        delete m_displayWindow;
    delete ui;
}

void MainWindow::slotSwitchCurrentPage(int pageIndex)
{
    //0-JointMotionScope_Page
    //1-TreatMode_Page
    //2-PassiveTraining_Page
    //3-BoostTraining_Page
    //4-ActiveTraining_Page
    //5-MemoryTraining_Page
    //6-TrainingGame_Page
    //7-Evaluation_Page
    //8-StrengthEvaluation_Page
    //9-StrengthEvaluationResult_Page
    //10-MotionScopeEvaluation_Page
    //11-MotionScopeEvaluationResult_Page
    //12-TrainingRecord_Page
    //13-Empty_Page
    if(0 == pageIndex)
    {
        ui->JointMotionScope_Page->setFingersState(CurrentUserData::getInstace()->getCurrentUserHandData());
    }
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::initWidgetUI()
{
    //????????????
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setYOffset(2);
    shadowEffect->setBlurRadius(12);
    shadowEffect->setColor(QColor(0,0,0,51));
    ui->title_Widget->setGraphicsEffect(shadowEffect);
    //???????????? ???????????????Z???
    ui->title_Widget->raise();

    initButtonGroup();

    //?????????????????????
    m_emergencyLabel = new QLabel(this);
    m_emergencyLabel->move(1550,50);
    m_emergencyLabel->resize(120,120);
    m_emergencyLabel->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap("./source/dialog/emergency.png");
    m_emergencyLabel->setPixmap(pixmap);
    m_emergencyLabel->hide();

    m_connectStateLabel = new QLabel(this);
    m_connectStateLabel->move(1410,50);
    m_connectStateLabel->resize(120,120);
    QPixmap connectPixmap("./source/dialog/connect.png");
    m_connectStateLabel->setPixmap(connectPixmap);
    m_connectStateLabel->hide();
}

void MainWindow::initButtonGroup()
{
    m_buttonGroup = new QButtonGroup();
    //??????????????????
    m_buttonGroup->setExclusive(true);
    m_buttonGroup->addButton(ui->jointScope_Btn,0);
    m_buttonGroup->addButton(ui->treatMode_Btn,1);
//    m_buttonGroup->addButton(ui->trainingGame_Btn,2);
    m_buttonGroup->addButton(ui->evaluation_Btn,3);
    m_buttonGroup->addButton(ui->trainingRecord_Btn,4);
    m_buttonGroup->addButton(ui->setting_Btn,5);
    m_buttonGroup->addButton(ui->shutDown_Btn,6);

    m_checkButtonGroup = new QButtonGroup();
    m_checkButtonGroup->addButton(ui->thumb_Btn,0);
    m_checkButtonGroup->addButton(ui->forefinger_Btn,1);
    m_checkButtonGroup->addButton(ui->midd_Btn,2);
    m_checkButtonGroup->addButton(ui->ringfinger_Btn,3);
    m_checkButtonGroup->addButton(ui->little_Btn,4);
    m_checkButtonGroup->addButton(ui->rightThumb_Btn,5);

    connect(m_checkButtonGroup,&QButtonGroup::idClicked,this,&MainWindow::slotCheckbuttonClicked);
}

void MainWindow::setBtnsLcok(bool isLock)
{
    foreach(QAbstractButton* button,m_buttonGroup->buttons())
    {
        QPushButton *tempBtn =  qobject_cast<QPushButton*>(button);
        if((tempBtn != ui->jointScope_Btn) && (tempBtn != ui->shutDown_Btn) /*&& (tempBtn != ui->treatMode_Btn)*/)
        {
            tempBtn->setEnabled(!isLock);
            //            tempBtn->setStyleSheet("QPushButton:hover{color: black;background: #E4F4FC;}"
            //                                   "QPushButton{color: black;background: white;}");
        }
    }
}

void MainWindow::slotBtnsLock(bool isLock)
{
    setBtnsLcok(isLock);
}

//??????????????????????????????????????????
void MainWindow::slotBtnGroupClicked(int btnId)
{
    //????????????????????????????????????????????????????????????????????????
    QPushButton *selectedBtn = qobject_cast<QPushButton*>(m_buttonGroup->button(btnId));
    selectedBtn->setStyleSheet("QPushButton{color: white;background: #0D9DDB;}");
    foreach(QAbstractButton* button,m_buttonGroup->buttons())
    {
        QPushButton *tempBtn =  qobject_cast<QPushButton*>(button);
        if(tempBtn != selectedBtn)
        {
            tempBtn->setStyleSheet("QPushButton:hover{color: black;background: #E4F4FC;}"
                                   "QPushButton{color: black;background: white;}");
        }
    }
}

void MainWindow::slotCheckbuttonClicked(int id)
{
    int force = 0;
    switch(id)
    {
    case 0:
        force = ui->thumb_lineEdit->text().toInt();
        break;
    case 1:
        force = ui->forefinger_lineEdit->text().toInt();
        break;
    case 2:
        force = ui->midd_lineEdit->text().toInt();
        break;
    case 3:
        force = ui->ringfinger_lineEdit->text().toInt();
        break;
    case 4:
        force = ui->little_lineEdit->text().toInt();
        break;
    case 5:
        force = ui->rightThumb_lineEdit->text().toInt();
        break;
    }

    ST_SetForceStander st_setForceStander;
    st_setForceStander.fingerIndex = id;
    st_setForceStander.force = force;


    uint16_t cmdId = STANDARDIZATION_VALUE;
    uint8_t seq = 0;
    uint8_t myid = 1;
    QByteArray array;
    array.resize(sizeof(ST_SetForceStander));
    memcpy(array.data(),&st_setForceStander,sizeof(st_setForceStander));
    CCommunicateAPI::getInstance()->sendData(seq,myid,cmdId,array);
}

void MainWindow::on_jointScope_Btn_clicked()
{
    //???????????????????????????ID
    if(JointMotionScopePage_E == MainWindowPageControl::getInstance()->getCurrentPageIndex())
        return;
    else //????????????????????????????????????????????????
    {
        //???????????????????????????
        bool flag = MainWindowPageControl::getInstance()->getPageFlag((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
        if(flag) //????????????
        {
            //???????????????,???????????????????????????????????????
            bool state = true;
            QMessageBox msgBox;
            QPushButton *cancelBtn = msgBox.addButton(tr("??????"),QMessageBox::ActionRole);
            QPushButton *confirmlBtn = msgBox.addButton(tr("??????"),QMessageBox::ActionRole);
            msgBox.setText(tr("?????????????????????????????????????????????"));
            msgBox.exec();
            if(msgBox.clickedButton() == cancelBtn)
            {
                state = false;
            }
            else if(msgBox.clickedButton() == confirmlBtn)
            {
                state = true;
            }
            //(1)????????????,??????????????????
            if(state)
            {
                //??????????????????
                slotBtnGroupClicked(0);
                //??????????????????????????????????????????
                MainWindowPageControl::getInstance()->signalTurnOffRunning((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
                //??????????????????????????????
                MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);
            }
            //(2)????????????
            else
                return;
        }
        else //?????????
        {
            //??????????????????
            slotBtnGroupClicked(0);
            MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);
        }
    }
}

void MainWindow::on_treatMode_Btn_clicked()
{
    //???????????????????????????ID
    if(TreatModePage_E == MainWindowPageControl::getInstance()->getCurrentPageIndex())
        return;
    else
    {
        //???????????????????????????
        bool flag = MainWindowPageControl::getInstance()->getPageFlag((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
        if(flag) //????????????
        {
            //???????????????,???????????????????????????????????????
            bool state = true;
            QMessageBox msgBox;
            QPushButton *cancelBtn = msgBox.addButton(tr("??????"),QMessageBox::ActionRole);
            QPushButton *confirmlBtn = msgBox.addButton(tr("??????"),QMessageBox::ActionRole);
            msgBox.setText(tr("?????????????????????????????????????????????"));
            msgBox.exec();
            if(msgBox.clickedButton() == cancelBtn)
            {
                state = false;
            }
            else if(msgBox.clickedButton() == confirmlBtn)
            {
                state = true;
            }
            //(1)????????????,??????????????????
            if(state)
            {
                //??????????????????
                slotBtnGroupClicked(1);
                //??????????????????????????????????????????
                MainWindowPageControl::getInstance()->signalTurnOffRunning((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
                //??????????????????????????????
                MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);
            }
            //(2)????????????
            else
                return;
        }
        else //?????????
        {
            //??????????????????
            slotBtnGroupClicked(1);
            MainWindowPageControl::getInstance()->setCurrentPage(TreatModePage_E);
        }
    }
}

void MainWindow::on_trainingGame_Btn_clicked()
{
    slotBtnGroupClicked(2);
    MainWindowPageControl::getInstance()->setCurrentPage(TrainingGamePage_E);
}

void MainWindow::on_evaluation_Btn_clicked()
{
    //?????????????????????????????????
    if(EvaluationPage_E == MainWindowPageControl::getInstance()->getCurrentPageIndex())
        return;
    else
    {
        //???????????????????????????
        bool flag = MainWindowPageControl::getInstance()->getPageFlag((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
        if(flag) //????????????
        {
            //???????????????,???????????????????????????????????????
            bool state = true;
            //(1)????????????,??????????????????
            if(state)
            {
                //??????????????????
                slotBtnGroupClicked(3);
                //??????????????????????????????????????????
                MainWindowPageControl::getInstance()->signalTurnOffRunning((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
                //??????????????????????????????
                MainWindowPageControl::getInstance()->setCurrentPage(EvaluationPage_E);
            }
            //(2)????????????
            else
                return;
        }
        else //?????????
        {
            //??????????????????
            slotBtnGroupClicked(3);
            MainWindowPageControl::getInstance()->setCurrentPage(EvaluationPage_E);
        }
    }
}

void MainWindow::on_trainingRecord_Btn_clicked()
{
    if(TrainingRecordPage_E == MainWindowPageControl::getInstance()->getCurrentPageIndex())
        return;
    else
    {
        //???????????????????????????
        bool flag = MainWindowPageControl::getInstance()->getPageFlag((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
        if(flag) //????????????
        {
            //???????????????,???????????????????????????????????????
            bool state = true;
            //(1)????????????,??????????????????
            if(state)
            {
                //??????????????????
                slotBtnGroupClicked(4);
                //??????????????????????????????????????????
                MainWindowPageControl::getInstance()->signalTurnOffRunning((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
                //??????????????????????????????
                MainWindowPageControl::getInstance()->setCurrentPage(TrainingRecordPage_E);
            }
            //(2)????????????
            else
                return;
        }
        else //?????????
        {
            //??????????????????
            slotBtnGroupClicked(4);
            MainWindowPageControl::getInstance()->setCurrentPage(TrainingRecordPage_E);
        }
    }
}

void MainWindow::on_setting_Btn_clicked()
{
    //m_displayWindow->show();

    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(3 == result)
    {
        if(SettingPage_E == MainWindowPageControl::getInstance()->getCurrentPageIndex())
            return;
        else
        {
            //???????????????????????????
            bool flag = MainWindowPageControl::getInstance()->getPageFlag((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
            if(flag) //????????????
            {
                //???????????????,???????????????????????????????????????
                bool state = true;
                //(1)????????????,??????????????????
                if(state)
                {
                    //??????????????????
                    slotBtnGroupClicked(5);
                    //??????????????????????????????????????????
                    MainWindowPageControl::getInstance()->signalTurnOffRunning((E_PAGENAME)(MainWindowPageControl::getInstance()->getCurrentPageIndex()));
                    //??????????????????????????????
                    MainWindowPageControl::getInstance()->setCurrentPage(SettingPage_E);
                }
                //(2)????????????
                else
                    return;
            }
            else //?????????
            {
                //??????????????????
                slotBtnGroupClicked(5);
                MainWindowPageControl::getInstance()->setCurrentPage(SettingPage_E);
            }
        }
    }
    else
        return;

}

void MainWindow::on_shutDown_Btn_clicked()
{
    this->close();
}

void MainWindow::on_switchUser_Btn_clicked()
{
    switchUser();
}

void MainWindow::switchUser()
{
    if(m_userManager)
    {
        m_userManager->show();
    }
}

void MainWindow::setCurrentUser()
{
    ST_PatientMsg st_PatientMsg = CurrentUserData::getInstace()->getTempUserMsg();
    QString pixPath;
    if(0 == st_PatientMsg.sex)//male
        pixPath = ":/new/prefix1/source/image/male.png";
    else if(1 == st_PatientMsg.sex)//female
        pixPath = ":/new/prefix1/source/image/female.png";
    QPixmap pix(pixPath);
    ui->portrait_Label->setPixmap(pix);

    ui->ID_Label->setText(QString(tr("ID???:%1")).arg(st_PatientMsg.ID));
    ui->name_Label->setText(QString(tr("??????:%1")).arg(st_PatientMsg.name));

}

void MainWindow::slotCurrentUserChanged()
{
    setCurrentUser();
    //???????????????????????????????????????????????????
    slotBtnGroupClicked(0);
}

//???????????????
void MainWindow::slotHeartBeatSlot()
{
    CCommunicateAPI::getInstance()->sendHeartBeat();
    //????????????
    m_heartbeatLostTimes++;
    if(m_heartbeatLostTimes > 10) //????????????
    {
        m_connectState = false;
        if(m_heartbeatLostTimes > 100000000)
            m_heartbeatLostTimes = 0;
        if(!m_disconnectDialog->getPressedState())
            m_disconnectDialog->show();
    }

    if(m_lastConnectState==false &&  m_connectState)
    {
        m_disconnectDialog->close();
    }
    m_lastConnectState = m_connectState;

    //??????????????????
    static int times = 0;
    if(m_isInEmergency)
    {
        m_isInEmergency = false;
        if(m_emergencyLabel->isVisible())
            m_emergencyLabel->hide();
        else
            m_emergencyLabel->show();
    }
    else
    {
        times++;
        if(times >4)
        {
            m_emergencyLabel->hide();
            times = 0;
        }
    }
    //????????????????????????
    if(!m_connectState)
    {
        if(m_connectStateLabel->isVisible())
            m_connectStateLabel->hide();
        else
            m_connectStateLabel->setVisible(true);
    }
    else
        m_connectStateLabel->hide();
}

void MainWindow::slotReceiveData(QByteArray receiveData)
{
    int length = receiveData[1];
    QByteArray data = receiveData.mid(5,length);
    uint16_t comID;
    memcpy(&comID,receiveData.data()+4,2);

    switch(comID)
    {
    case HEARTBEAT:
        m_heartbeatLostTimes = 0;
        m_connectState = true;
        break;
    case CHECKRESULT:
        break;
    case EMERGENCY:
        m_isInEmergency = true;
        break;
    case SPASMWARNING:

        break;
    }
}

void MainWindow::slotWatchSend(QByteArray array)
{
    m_displayWindow->setText(array);
}

//????????????
void MainWindow::on_startCheck_Btn_clicked()
{
    uint16_t cmdId = START_SENSOR_CHECK;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//????????????
void MainWindow::on_stopCheck_Btn_clicked()
{
    uint16_t cmdId = STOP_SENSOR_CHECK;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(0);
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

