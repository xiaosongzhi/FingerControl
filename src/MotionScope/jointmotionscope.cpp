#include "jointmotionscope.h"
#include "ui_jointmotionscope.h"
#include "currentuserdata.h"
#include <QButtonGroup>
#include <QDebug>
#include "ccommunicateapi.h"
#include "cdatabaseinterface.h"
#include "mainwindowpagecontrol.h"
#include <QMapIterator>
#include "readconfig.h"
#include <QMessageBox>
JointMotionScope::JointMotionScope(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JointMotionScope),
    btnGroupBox(NULL),
    resetBtnGroup(NULL),
    m_handType(0),
    m_MachineCheckDialog(NULL),
    isTesting(false),
    m_isMachineChecked(false)
{
    ui->setupUi(this);
    m_MachineCheckDialog = new WarningDialog();
    m_MachineCheckDialog->setWarningType(CHECKING_E);
    m_MoveTipDialog = new WarningDialog();
    m_MoveTipDialog->setWarningType(MOVETIPS_E);
    m_touchTipDialog  = new WarningDialog();
    m_touchTipDialog->setWarningType(TOUCHWARNING_E);
    initWidget();
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(receiveCommData(QByteArray)));
    connect(ui->leftHand_Widget,SIGNAL(singnalFingerValueChanged(int,int,int)),this,SLOT(slotFingerValueChanged(int,int,int)));
    connect(ui->rightHand_Widget,SIGNAL(singnalFingerValueChanged(int,int,int)),this,SLOT(slotFingerValueChanged(int,int,int)));
    connect(m_MoveTipDialog,SIGNAL(signalConfirmClicked(E_WarningType)),this,SLOT(slotWarningType(E_WarningType)));
    connect(MainWindowPageControl::getInstance(),SIGNAL(signalTurnOffRunning(int)),this,SLOT(slotTurnOffRunning(int)));

    connect(CurrentUserData::getInstace(),SIGNAL(signalUserChanged()),this,SLOT(slotCurrentUserChanged()));
    if(0 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(true);
    else if(1 == ReadConfig::getInstance()->getProgramMode())
        setTestVisible(false);

    ui->LTouch_Label->hide();
    ui->RTouch_Label->hide();
    ui->lock_Btn->setVisible(false);
}

JointMotionScope::~JointMotionScope()
{
    if(btnGroupBox)
    {
        btnGroupBox->deleteLater();
        btnGroupBox = NULL;
    }
    if(resetBtnGroup)
    {
        resetBtnGroup->deleteLater();
        resetBtnGroup = NULL;
    }
    if(m_MachineCheckDialog)
    {
        delete  m_MachineCheckDialog;
        m_MachineCheckDialog = NULL;
    }

    delete ui;
}

void JointMotionScope::initWidget()
{
    btnGroupBox = new QButtonGroup();
    btnGroupBox->setExclusive(true);
    btnGroupBox->addButton(ui->allOut_Btn,0);
    btnGroupBox->addButton(ui->fourInOneOut_Btn,1);
    btnGroupBox->addButton(ui->fourOutOneIn_Btn,2);
    btnGroupBox->addButton(ui->lock_Btn,3);
    connect(btnGroupBox,SIGNAL(buttonClicked(int)),this,SLOT(slotBtnGroupClicked(int)));

    resetBtnGroup = new QButtonGroup();
    resetBtnGroup->addButton(ui->reset0L_Btn,0);
    resetBtnGroup->addButton(ui->reset1L_Btn,1);
    resetBtnGroup->addButton(ui->reset2L_Btn,2);
    resetBtnGroup->addButton(ui->reset3L_Btn,3);
    resetBtnGroup->addButton(ui->reset4L_Btn,4);
    resetBtnGroup->addButton(ui->reset0R_Btn,5);
    resetBtnGroup->addButton(ui->reset1R_Btn,6);
    resetBtnGroup->addButton(ui->reset2R_Btn,7);
    resetBtnGroup->addButton(ui->reset3R_Btn,8);
    resetBtnGroup->addButton(ui->reset4R_Btn,9);

    connect(resetBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotResetBtnGroupClicked(int)));
    ui->allOut_Btn->setChecked(true);

    ui->leftHand_Widget->switchHand(0);
    ui->rightHand_Widget->switchHand(1);


}

void JointMotionScope::slotBtnGroupClicked(int btnId)
{
    switch (btnId) {
    case 0: //所有手指向外
    {
        ui->allOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/allOutSelect.png);}");
        ui->fourInOneOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourInOneOurNormal.png);} \
                                            QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/fourInOneOUtHover.png);}");
                                            ui->fourOutOneIn_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourOutOneInNormal.png);}\
                                                                                QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/fourOutOneInHover.png);}");
        }
        break;
    case 1: //四指向内一指向外
    {
        ui->fourInOneOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourInOneOutSelect.png);}");
        ui->allOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/allOutMormal.png);}\
                                      QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/allOutHover.png);}");
                                      ui->fourOutOneIn_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourOutOneInNormal.png);}\
                                                                          QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/fourOutOneInHover.png);}");

        }
        break;
    case 2: //四指向外一指向内
    {
        ui->fourOutOneIn_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourOutOneInSelect.png);}");
        ui->allOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/allOutMormal.png);}\
                                      QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/allOutHover.png);}");

                                      ui->fourInOneOut_Btn->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/source/jointScope/fourInOneOurNormal.png);} \
                                                                          QPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/fourInOneOUtHover.png);}");
        }
        break;
    case 3: //锁定按钮
    {
        QString unlock("QPushButton{border-image: url(:/new/prefix1/source/jointScope/unlockNormal.png);}\nQPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/unlockHover.png);}");
        QString lock("QPushButton{border-image: url(:/new/prefix1/source/jointScope/lock.png);}");
        if(ui->lock_Btn->styleSheet() == lock)
        {
            ui->lock_Btn->setStyleSheet(unlock);
        }
        else
        {
            ui->lock_Btn->setStyleSheet(lock);
        }

    }
        break;

    }
}


void JointMotionScope::on_reset_Btn_clicked()
{
    if(0 == m_handType)
    {
        for(int i = 0;i < 5;i++)
        {
            //跳过禁用的按钮
            if(st_HandData.fingerSelected[i])
            slotResetBtnGroupClicked(i);
        }
    }
    else if(1 == m_handType)
    {
        for(int i = 5;i<10;i++)
        {
            //跳过禁用的按钮
            if(st_HandData.fingerSelected[i%5])
            {
               slotResetBtnGroupClicked(i);
            }
        }
    }
    resetTrainTest();
    Sleep(100);
    CCommunicateAPI::getInstance()->getRealTimePositon(true);
}

void JointMotionScope::resetTrainTest()
{
    //更新状态
    MainWindowPageControl::getInstance()->updatePageRunningflag(JointMotionScopePage_E,true);
    QByteArray sendArray;
    sendArray.resize(0);
    uint16_t cmdId = RESET_TRAIN_TEST;
    uint8_t seq = 0;
    uint8_t id = 1;
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,sendArray);
}

void JointMotionScope::on_save_Btn_clicked()
{
    //获取手指活动范围
    QMap<int,QPair<int,int>> fingersRangeMap;
    if(0 == m_handType)
    {
        fingersRangeMap = ui->leftHand_Widget->getFingersRange();
        CurrentUserData::getInstace()->setFingerOppositeRange(ui->leftHand_Widget->getOppositeRange());
    }
    else if(1 == m_handType)
    {
        fingersRangeMap = ui->rightHand_Widget->getFingersRange();
        CurrentUserData::getInstace()->setFingerOppositeRange(ui->rightHand_Widget->getOppositeRange());
    }
    //当有接触风险时，给出弹窗提示(待测试)
    /*
    if(fingersRangeMap.size() > 0)
    {
        QPair<int,int> rangePair = fingersRangeMap[0];
        if(rangePair.first > 48)
        {
            m_MachineCheckDialog->show();
            return;
        }
    }
    */
    //更新状态
    MainWindowPageControl::getInstance()->updatePageRunningflag(JointMotionScopePage_E,false);
    //停止获取实时位置
    CCommunicateAPI::getInstance()->getRealTimePositon(false);


    ST_HandData st_HandData = CurrentUserData::getInstace()->getTempUserHandData();
    ST_JointRange st_JointRange;
    for(int i = 0;i < st_HandData.fingers.size();i++)
    {
        if(0 == i)
            st_JointRange.fingers.append(QString::number(st_HandData.fingers.at(i)));
        else
        {
            st_JointRange.fingers.append("-");
            st_JointRange.fingers.append(QString::number(st_HandData.fingers.at(i)));
        }
    }
    st_JointRange.ID = CurrentUserData::getInstace()->getTempUserMsg().ID;
    st_JointRange.hand = st_HandData.hand;
    st_JointRange.fingersRange = fingersRangeMap;
    //将该用户设置为当前用户
    CurrentUserData::getInstace()->setCurrentUserHandData(st_HandData);
    //设置当前用户的手指数据
    CurrentUserData::getInstace()->setFingersRangeMap(fingersRangeMap);
    //将手指活动范围告知下位机
    Sleep(100);
    sendFingersRangeToSlave(fingersRangeMap);

    /********存入数据库***********/
    //(1)存入基本用户信息
    CDatabaseInterface *dataBaseinstance = CDatabaseInterface::getInstance();

    ST_PatientMsg st_PatientMsg = CurrentUserData::getInstace()->getTempUserMsg();
    QString tableName("patientmessagetable");

    //相当于hands
    int whichHand = -1;

    //先查询是否存在该用户
    QString query(QString("select * from %1 where ID ='%2'").arg(tableName).arg(st_JointRange.ID));

    ST_PatientMsg st_CheckPatientMsg;
    //存在则更新
    if(dataBaseinstance->exec(query))
    {
        if(dataBaseinstance->getValuesSize() >0)
        {
            st_CheckPatientMsg = variantMapToPatientMsg(dataBaseinstance->getValues(0,1).at(0));

            //之前已被录入过
            if(st_CheckPatientMsg.hands == st_HandData.hand)
                whichHand = st_HandData.hand;
            //新增一只手，变为两只
            else
                whichHand = 2;
            st_PatientMsg.hands = whichHand;
            QVariantMap vMap = patientMsgToVariantMap(st_PatientMsg);
            if(!dataBaseinstance->updateRowTable(tableName,"ID",vMap))
                qDebug()<<"patientmessagetable更新失败";
            else
                qDebug()<<"patientmessagetable更新成功";
        }
        //不存在则添加
        else
        {
            whichHand = st_HandData.hand;
            st_PatientMsg.hands = whichHand;
            QVariantMap vMap = patientMsgToVariantMap(st_PatientMsg);
            if(!dataBaseinstance->insertRowTable(tableName,vMap))
            {
                qDebug()<<dataBaseinstance->getLastError();
                return;
            }
            else
            {
                //更新用户管理界面
                qDebug()<<"基本信息插入成功";
            }
        }
    }
    else
        qDebug()<<dataBaseinstance->getLastError();

    //(2)存入手指活动度信息
    QVariantMap rangMap = jointRangeToVariantMap(st_JointRange);
    QString jointTable("jointrangetable");
    //此处也不能直接插入，需要先查询判断是否存在，如果存在则更新，不存在则添加
    QString queryRange(QString("select * from %1 where ID = '%2' and hand = '%3'").arg(jointTable).arg(st_JointRange.ID).arg(st_HandData.hand));
    if(dataBaseinstance->exec(queryRange))
    {
        //存在则更新
        int size = dataBaseinstance->getValuesSize();
        if( size > 0)
        {
            if(dataBaseinstance->updateRowTable(jointTable,"ID",QString::number(st_JointRange.ID),"hand",rangMap))
            {
                //解锁其他按钮
                MainWindowPageControl::getInstance()->setMainWindowBtnsLock(false);
                qDebug()<<"更新成功";
            }

            else
                qDebug()<<"更新失败"<<dataBaseinstance->getLastError();
        }
        //不存在添加
        else
        {
            if(dataBaseinstance->insertRowTable(jointTable,rangMap))
            {
                //解锁其他按钮
                MainWindowPageControl::getInstance()->setMainWindowBtnsLock(false);
                qDebug()<<"关节活动度设定值插入成功";
            }
            else
            {
                qDebug()<<"关节活动度设定值插入失败";
                qDebug()<<dataBaseinstance->getLastError();
                return;
            }
        }
    }
    else
        qDebug()<<dataBaseinstance->getLastError();

    //更新训练用户数据
    CurrentUserData::getInstace()->updateTrainUser();

    QMessageBox::warning(NULL,tr("提示!"),tr("保存成功"),QMessageBox::Ok);
}

void JointMotionScope::on_allOut_Btn_clicked()
{
    ST_FingersPosition st_FingersPosition;
    //获取手指活动范围
    //  手指范围      活动度
    QMap<int,QPair<int,int>> fingersRangeMap;
    if(0 == m_handType)
    {
        fingersRangeMap = ui->leftHand_Widget->getFingersRange();
    }
    else if(1 == m_handType)
    {
        fingersRangeMap = ui->rightHand_Widget->getFingersRange();
    }

    if(fingersRangeMap.contains(0) && st_HandData.fingers.contains(0))
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).second;
        else if(1 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).first;
    }
    else
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = -1;
        else if(1 == m_handType)
            st_FingersPosition.thumb = -1;
    }

    if(fingersRangeMap.contains(1) && st_HandData.fingers.contains(1))
        st_FingersPosition.forefinger = fingersRangeMap.value(1).second;
    else
        st_FingersPosition.forefinger = -1;

    if(fingersRangeMap.contains(2) && st_HandData.fingers.contains(2))
        st_FingersPosition.middlefinger = fingersRangeMap.value(2).second;
    else
        st_FingersPosition.middlefinger = -1;

    if(fingersRangeMap.contains(3) && st_HandData.fingers.contains(3))
        st_FingersPosition.ringfinger = fingersRangeMap.value(3).second;
    else
        st_FingersPosition.ringfinger = -1;
    if(fingersRangeMap.contains(4) && st_HandData.fingers.contains(4))
        st_FingersPosition.littlefinger = fingersRangeMap.value(4).second;
    else
        st_FingersPosition.littlefinger = -1;

    //设置四指的目标位置
    CCommunicateAPI::getInstance()->setFingersPositon(st_FingersPosition);
}

void JointMotionScope::on_fourInOneOut_Btn_clicked()
{
    ST_FingersPosition st_FingersPosition;
    //获取手指活动范围
    QMap<int,QPair<int,int>> fingersRangeMap;
    //左手
    if(0 == m_handType)
    {
        fingersRangeMap = ui->leftHand_Widget->getFingersRange();
    }
    //右手
    else if(1 == m_handType)
    {
        fingersRangeMap = ui->rightHand_Widget->getFingersRange();
    }
    //给拇指赋值
    if(fingersRangeMap.contains(0) && st_HandData.fingers.contains(0))
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).second;
        else if(1 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).first;
    }
    else
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = -1;
        else if(1 == m_handType)
            st_FingersPosition.thumb = -1;
    }

    if(fingersRangeMap.contains(1) && st_HandData.fingers.contains(1))
        st_FingersPosition.forefinger = fingersRangeMap.value(1).first;
    else
        st_FingersPosition.forefinger = -1;

    if(fingersRangeMap.contains(2) && st_HandData.fingers.contains(2))
        st_FingersPosition.middlefinger = fingersRangeMap.value(2).first;
    else
        st_FingersPosition.middlefinger = -1;

    if(fingersRangeMap.contains(3) && st_HandData.fingers.contains(3))
        st_FingersPosition.ringfinger = fingersRangeMap.value(3).first;
    else
        st_FingersPosition.ringfinger = -1;
    if(fingersRangeMap.contains(4) && st_HandData.fingers.contains(4))
        st_FingersPosition.littlefinger = fingersRangeMap.value(4).first;
    else
        st_FingersPosition.littlefinger = -1;

    //设置四指的目标位置
    CCommunicateAPI::getInstance()->setFingersPositon(st_FingersPosition);
}

//四指向外，伸展动作
void JointMotionScope::on_fourOutOneIn_Btn_clicked()
{
    ST_FingersPosition st_FingersPosition;
    //获取手指活动范围
    QMap<int,QPair<int,int>> fingersRangeMap;
    if(0 == m_handType)
    {
        fingersRangeMap = ui->leftHand_Widget->getFingersRange();
    }
    else if(1 == m_handType)
    {
        fingersRangeMap = ui->rightHand_Widget->getFingersRange();
    }

    if(fingersRangeMap.contains(0) && st_HandData.fingers.contains(0))
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).first;
        else if(1 == m_handType)
            st_FingersPosition.thumb = fingersRangeMap.value(0).second;
    }
    else
    {
        if(0 == m_handType)
            st_FingersPosition.thumb = -1;
        else if(1 == m_handType)
            st_FingersPosition.thumb = -1;
    }

    if(fingersRangeMap.contains(1) && st_HandData.fingers.contains(1))
        st_FingersPosition.forefinger = fingersRangeMap.value(1).second;
    else
        st_FingersPosition.forefinger = -1;

    if(fingersRangeMap.contains(2) && st_HandData.fingers.contains(2))
        st_FingersPosition.middlefinger = fingersRangeMap.value(2).second;
    else
        st_FingersPosition.middlefinger = -1;

    if(fingersRangeMap.contains(3) && st_HandData.fingers.contains(3))
        st_FingersPosition.ringfinger = fingersRangeMap.value(3).second;
    else
        st_FingersPosition.ringfinger = -1;
    if(fingersRangeMap.contains(4) && st_HandData.fingers.contains(4))
        st_FingersPosition.littlefinger = fingersRangeMap.value(4).second;
    else
        st_FingersPosition.littlefinger = -1;

    //设置四指的目标位置
    CCommunicateAPI::getInstance()->setFingersPositon(st_FingersPosition);
}

//切换左右手
void JointMotionScope::switchHand(int type)
{
    m_handType = type;
    ui->stackedWidget->setCurrentIndex(type);
    ui->leftHand_Widget->switchHand(type);
}

//重置各个按钮
void JointMotionScope::slotResetBtnGroupClicked(int btnId)
{
    int fingerIndex = btnId % 5;
    int handIndex = btnId / 5;
    if(0 == handIndex)
        ui->leftHand_Widget->resetHandFingerValue(handIndex,fingerIndex);
    else if(1 == handIndex)
        ui->rightHand_Widget->resetHandFingerValue(handIndex,fingerIndex);
}

void JointMotionScope::on_L_Btn_clicked()
{
//    if(ui->L_Btn->text() == tr("位置开"))
//    {
//        CCommunicateAPI::getInstance()->getRealTimePositon(true);
//        ui->L_Btn->setText(tr("位置关"));
//    }
//    else if(ui->L_Btn->text() == tr("位置关"))
//    {
//        CCommunicateAPI::getInstance()->getRealTimePositon(false);
//        ui->L_Btn->setText(tr("位置开"));
//    }

}

void JointMotionScope::on_R_Btn_clicked()
{
//    if(ui->R_Btn->text() == tr("力量开"))
//    {
//        CCommunicateAPI::getInstance()->getRealTimeStrength(true);
//        ui->R_Btn->setText(tr("力量关"));
//    }
//    else if(ui->R_Btn->text() == tr("力量关"))
//    {
//        CCommunicateAPI::getInstance()->getRealTimeStrength(false);
//        ui->R_Btn->setText(tr("力量开"));
//    }
}

void JointMotionScope::setFingerEnable(int handType,int fingerIndex,bool isEnable)
{
    setResetBtnEnable(handType,fingerIndex,isEnable);
    switch(handType)
    {
    case 0:
    {
        ui->leftHand_Widget->setHandFingerEnable(handType,fingerIndex,isEnable);
    }
        break;
    case 1:
    {
        ui->rightHand_Widget->setHandFingerEnable(handType,fingerIndex,isEnable);
    }
        break;
    }
}

void JointMotionScope::setResetBtnEnable(int handType,int fingerIndex,bool isEnable)
{
    QString normalStyle = "QPushButton{border-image: url(:/new/prefix1/source/image/resetNormal.png);}\n"
                          "QPushButton:hover{border-image: url(:/new/prefix1/source/image/resetHover.png);}";
    QString disableStyle = "QPushButton{border-image: url(:/new/prefix1/source/image/resetDisbale.png);}";
    QString btnName;
    if(0 == handType)
        btnName = QString("reset%1%2_Btn").arg(fingerIndex).arg("L");
    else if(1 == handType)
        btnName = QString("reset%1%2_Btn").arg(fingerIndex).arg("R");
    foreach(QAbstractButton* btn,resetBtnGroup->buttons())
    {
        QPushButton *tempBtn = static_cast<QPushButton *>(btn);
        if(tempBtn->objectName() == btnName)
        {
            if(isEnable)
                tempBtn->setStyleSheet(normalStyle);
            else
                tempBtn->setStyleSheet(disableStyle);

            tempBtn->setEnabled(isEnable);
        }
    }
}

//设置用户手指状态数据
void JointMotionScope::setFingersState(ST_HandData st_Hand)
{
    //切换手
    switchHand(st_Hand.hand);
    //选中手指
    for(int i = 0;i < 5;i++)
    {
        if(st_Hand.fingers.contains(i))
            setFingerEnable(st_Hand.hand,i,true);
        else
            setFingerEnable(st_Hand.hand,i,false);
    }
}

void JointMotionScope::receiveCommData(QByteArray receiveData)
{
    int length = receiveData[1];
    QByteArray data = receiveData.mid(5,length);
    uint16_t comID;
    memcpy(&comID,receiveData.data()+4,2);

    switch(comID)
    {
    case MACHINEZEROCHECK: //收到机械零点校准完成指令
    {
        switch(receiveData[6])
        {
        case 1://校准中
        {
            m_MachineCheckDialog->show();
        }
            break;
        case 2://四指校准成功
        {
            //发送拇指校准指令
            machineCheck(2);
        }
            break;
        case 3://校准失败
        {
            qDebug()<<"机械校准失败";
        }
            break;
        case 4:
        {
            qDebug()<<"拇指机械校准中";
        }
            break;
        case 5:
        {
            //拇指校准成功
            //收到机械校准完成指令，关闭校准界面
            m_MachineCheckDialog->close();
            on_reset_Btn_clicked();
        }
            break;
        case 6:
        {
            qDebug()<<"拇指校准失败";
        }
            break;
        }
    }
        break;
    case RESPONDPOSITION: //获取手指实时位置数据
    {
        ST_RealTimeFingersPosition st_RealTimePosition;
        memcpy(&st_RealTimePosition,receiveData.data() + 6,sizeof(ST_RealTimeFingersPosition));
        setHandFingersPosition(st_RealTimePosition);
//        if(0 == m_handType)
//            ui->PoThumb_Label->setText(QString::number(st_RealTimePosition.Lthumb));
//        else if(1 == m_handType)
//            ui->PoThumb_Label->setText(QString::number(st_RealTimePosition.Rthumb));
//        ui->PoFore_Label->setText(QString::number(st_RealTimePosition.forefinger));
//        ui->PoMiddle_Label->setText(QString::number(st_RealTimePosition.middlefinger));
//        ui->PoRing_Label->setText(QString::number(st_RealTimePosition.ringfinger));
//        ui->PoLittle_Label->setText(QString::number(st_RealTimePosition.littlefinger));
    }
        break;
    case RESPONDFORCE://获取手指实时力量数据
    {
        ST_RealTimeStrength st_RealTimeStrength;
        memcpy(&st_RealTimeStrength,receiveData.data() + 6,sizeof(ST_RealTimeStrength));
//        ui->PwThumb_Label->setText(QString::number(st_RealTimeStrength.thumb));
//        ui->PwFore_Label->setText(QString::number(st_RealTimeStrength.forefinger));
//        ui->PwMiddle_Label->setText(QString::number(st_RealTimeStrength.middlefinger));
//        ui->PwRing_Label->setText(QString::number(st_RealTimeStrength.ringfinger));
//        ui->PwLittle_Label->setText(QString::number(st_RealTimeStrength.littlefinger));
    }
        break;
    case FINGERSRANGE:
    {
        qDebug()<<"关节活动度告知下位机成功";
    }
        break;
    }
}

void JointMotionScope::setHandFingersPosition(ST_RealTimeFingersPosition st_FingerPosition)
{
    if(0 == m_handType)
    {
        if(st_FingerPosition.Lthumb > 48)
            setTouchVisible(m_handType,true);
        else
            setTouchVisible(m_handType,false);
        ui->leftHand_Widget->setHandFingerValue(m_handType,0,100-st_FingerPosition.Lthumb);
        ui->leftHand_Widget->setHandFingerValue(m_handType,1,st_FingerPosition.forefinger);
        ui->leftHand_Widget->setHandFingerValue(m_handType,2,st_FingerPosition.middlefinger);
        ui->leftHand_Widget->setHandFingerValue(m_handType,3,st_FingerPosition.ringfinger);
        ui->leftHand_Widget->setHandFingerValue(m_handType,4,st_FingerPosition.littlefinger);
    }
    else if(1 == m_handType)
    {
        if(st_FingerPosition.Rthumb > 48)
            setTouchVisible(m_handType,true);
        else
            setTouchVisible(m_handType,false);
        ui->rightHand_Widget->setHandFingerValue(m_handType,0,st_FingerPosition.Rthumb);
        ui->rightHand_Widget->setHandFingerValue(m_handType,1,st_FingerPosition.forefinger);
        ui->rightHand_Widget->setHandFingerValue(m_handType,2,st_FingerPosition.middlefinger);
        ui->rightHand_Widget->setHandFingerValue(m_handType,3,st_FingerPosition.ringfinger);
        ui->rightHand_Widget->setHandFingerValue(m_handType,4,st_FingerPosition.littlefinger);
    }
}

void JointMotionScope::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    st_HandData = CurrentUserData::getInstace()->getTempUserHandData();
    //将左右手以及手指选中信息发送给下位机
    setHandTypeAndFinger(st_HandData);
    Sleep(100);
    //进入机械校准界面
//    enterMachineCheckPage();
    setFingersState(st_HandData);
}

//进入机械校准界面
void JointMotionScope::enterMachineCheckPage()
{
    if(m_isMachineChecked)
    {
        machineCheck(1);
        m_isMachineChecked = false;
    }
}

//发送手以及手指数据
void JointMotionScope::setHandTypeAndFinger(ST_HandData st_HandData)
{
    ST_HandTypeFingers st_HandFingers;

    st_HandFingers.handType = st_HandData.hand;
    if(st_HandData.fingers.contains(0))
        st_HandFingers.thumb = 1;
    else
        st_HandFingers.thumb = 0;
    if(st_HandData.fingers.contains(1))
        st_HandFingers.forfinger = 1;
    else
        st_HandFingers.forfinger = 0;
    if(st_HandData.fingers.contains(2))
        st_HandFingers.middlefinger = 1;
    else
        st_HandFingers.middlefinger = 0;
    if(st_HandData.fingers.contains(3))
        st_HandFingers.ringfinger = 1;
    else
        st_HandFingers.ringfinger = 0;
    if(st_HandData.fingers.contains(4))
        st_HandFingers.littlefinger = 1;
    else
        st_HandFingers.littlefinger = 0;

    QByteArray sendArray;
    sendArray.resize(sizeof(ST_HandTypeFingers));
    memcpy(sendArray.data(),&st_HandFingers,sendArray.size());

    uint16_t cmdId = HANDFINGERSELECTED;
    uint8_t seq = 0;
    uint8_t id = 1;

    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,sendArray);
}

//机械校准指令
void JointMotionScope::machineCheck(uint8_t type)
{
    if(1 == type)
        qDebug()<<"发送四指校准数据";
    else if(2 == type)
        qDebug()<<"发送拇指校准数据";
    uint16_t cmdId = MACHINEZEROCHECK;
    uint8_t seq = 0;
    uint8_t id = 1;
    QByteArray array;
    array.resize(1);
    array[0] = type;
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}

//手指上下箭头的控制
void JointMotionScope::slotFingerValueChanged(int handType,int fingerIndex,int upDown)
{
    Q_UNUSED(handType)
    uint16_t cmdId = CONTROLSIGLEFINGER;
    uint8_t seq = 0;
    uint8_t id = 1;
    ST_FingerUpDownControl st_FingerUpDownControl;
    st_FingerUpDownControl.fingerIndex = fingerIndex;
    st_FingerUpDownControl.direction = upDown;

    QByteArray array;
    array.resize(sizeof(ST_FingerUpDownControl));
    memcpy(array.data(),&st_FingerUpDownControl,array.size());
    CCommunicateAPI::getInstance()->sendData(seq,id,cmdId,array);
}


void JointMotionScope::on_lock_Btn_clicked()
{
    int8_t lockFlag = -1;
    QString unlock("QPushButton{border-image: url(:/new/prefix1/source/jointScope/unlockNormal.png);}\nQPushButton:hover{border-image: url(:/new/prefix1/source/jointScope/unlockHover.png);}");
    QString lock("QPushButton{border-image: url(:/new/prefix1/source/jointScope/lock.png);}");

    if(ui->lock_Btn->styleSheet() == unlock)
        lockFlag = 1;
    else if(ui->lock_Btn->styleSheet() == lock)
        lockFlag = 0;
    QByteArray arrayData;
    arrayData.resize(1);
    arrayData[0] = lockFlag;
    CCommunicateAPI::getInstance()->sendData(0,1,LOCKFINGERS,arrayData);
}

//将手指的活动范围发送给下位机
void JointMotionScope::sendFingersRangeToSlave(QMap<int,QPair<int,int>>& fingersRangeMap)
{
    QMapIterator<int,QPair<int,int>> iter(fingersRangeMap);
    ST_FingersRange st_FingersRange;
    int i = 0;
    while(iter.hasNext())
    {
        iter.next();
        st_FingersRange.fingersRange[2*i] = iter.value().second;
        st_FingersRange.fingersRange[2*i+1] = iter.value().first;
        i++;
    }
    QByteArray array;
    array.resize(sizeof(ST_FingersRange));
    memcpy(array.data(),&st_FingersRange,sizeof(ST_FingersRange));
    CCommunicateAPI::getInstance()->sendData(0,1,FINGERSRANGE,array);
}
void JointMotionScope::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    CCommunicateAPI::getInstance()->getRealTimePositon(false);
}

void JointMotionScope::slotTurnOffRunning(int pageIndex)
{
    if(pageIndex == JointMotionScopePage_E)
    {
        on_save_Btn_clicked();
    }
}


void JointMotionScope::setTestVisible(bool isShow)
{
//    ui->L_Btn->setVisible(isShow);
//    ui->R_Btn->setVisible(isShow);
//    ui->PoThumb_Label->setVisible(isShow);
//    ui->PwThumb_Label->setVisible(isShow);
//    ui->PoFore_Label->setVisible(isShow);
//    ui->PwFore_Label->setVisible(isShow);
//    ui->PoMiddle_Label->setVisible(isShow);
//    ui->PwMiddle_Label->setVisible(isShow);
//    ui->PoRing_Label->setVisible(isShow);
//    ui->PwRing_Label->setVisible(isShow);
//    ui->PoLittle_Label->setVisible(isShow);
//    ui->PwLittle_Label->setVisible(isShow);
}

void JointMotionScope::setTouchVisible(int type,bool isVisible)
{
    if(0 == type)
    {
        ui->LTouch_Label->setVisible(isVisible);
    }
    else if(1 == type)
    {
        ui->RTouch_Label->setVisible(isVisible);
    }
}

//切换用户 进行机械校准
void JointMotionScope::slotCurrentUserChanged()
{
    m_isMachineChecked = true;
    //弹出请将手指移开的提示
    m_MoveTipDialog->show();
}

//校准前的提示(手指已拿开)可以开始校准
void JointMotionScope::slotWarningType(E_WarningType type)
{
    if(MOVETIPS_E == type)
    {
        //进入校准界面
        enterMachineCheckPage();
    }
}
