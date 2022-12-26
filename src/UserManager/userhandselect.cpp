#include "userhandselect.h"
#include "ui_userhandselect.h"

#include "mainwindowpagecontrol.h"
#include "controlmaskdialog.h"
#include <QDebug>
#include "cdatabaseinterface.h"
UserHandSelect::UserHandSelect(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::UserHandSelect),
    m_handType(0),
    selectedHand(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    resize(1500,800);
    initWidget();
    connect(this,SIGNAL(signalCloseEvent()),this,SLOT(slotCloseBtn()));
}

UserHandSelect::~UserHandSelect()
{
    delete ui;
}

void UserHandSelect::initWidget()
{
    m_userFingerSelect = new UserFingerSelect();

    addStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/addNormal.png);}\n"
                             "QPushButton:hover{border-image: url(:/new/prefix1/source/selectHand/addHover.png);}");
    addStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/addSelected.png);}");

    leftHandStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/leftNormal.png);}\n"
                                  "QPushButton:hover{border-image: url(:/new/prefix1/source/selectHand/leftHover.png);}");
    leftHandStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/leftSelected.png);}");

    rightHandStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/rightNormal.png);}\n"
                                   "QPushButton:hover{border-image: url(:/new/prefix1/source/selectHand/rightHover.png);}");
    rightHandStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/selectHand/rightSelected.png);}");
}

//左侧按钮
void UserHandSelect::on_hand1_Btn_clicked()
{
    ST_HandData st_handData;
    //左侧按钮有两种可能  左手/右手
    if(0 == m_handType)//左手
    {
        //左手被选中
        selectedHand = 0;

        if(getHandData(selectedHand,st_handData))
        {
            CurrentUserData::getInstace()->setTempUserHandData(st_handData);
        }
        else
            qDebug()<<"获取手指数据失败";
        //进入左手关节活动度设定界面
        this->close();

        MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);

    }
    else if(1 == m_handType) //右手
    {
        //右手被选中
        selectedHand = 1;
        //进入右手关节活动度设定界面
        if(getHandData(selectedHand,st_handData))
        {
            CurrentUserData::getInstace()->setTempUserHandData(st_handData);
        }
        else
            qDebug()<<"获取手指数据失败";
        this->close();
        MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);

    }
    else if(2 == m_handType)
    {
        selectedHand = 0;
        if(getHandData(selectedHand,st_handData))
        {
            CurrentUserData::getInstace()->setTempUserHandData(st_handData);
        }
        else
            qDebug()<<"获取手指数据失败";
        this->close();
        MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);
    }
}

//右侧按钮
void UserHandSelect::on_hand2_Btn_clicked()
{
    ST_HandData st_handData;
    //右侧按钮有三种可能 添加/左手/右手
    //右侧为添加按钮
    if(0 == m_handType || 1 == m_handType)
    {
        //进入添加界面
        if(0 == m_handType)
            m_userFingerSelect->setLeftRightHand(1);
        if(1 == m_handType)
            m_userFingerSelect->setLeftRightHand(0);
        //先关窗口
        this->close();
        //再打开新窗口
        m_userFingerSelect->show();

    }
    else if(2 == m_handType)
    {
        //右手被选中
        selectedHand = 1;
        if(getHandData(selectedHand,st_handData))
        {
            CurrentUserData::getInstace()->setTempUserHandData(st_handData);
        }
        else
            qDebug()<<"获取手指数据失败";
        this->close();
        //进入右手关节活动度设定界面
        MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);
    }
}

void UserHandSelect::on_confirm_Btn_clicked()
{
    //选中手时进入关节活动度设定界面
    //选中添加时进入手指选择界面

}

void UserHandSelect::setBtnStyle(QPushButton* btn,QString style)
{
    btn->setStyleSheet(style);
}

void UserHandSelect::setHandBtns(int type)
{
    m_handType = type;
    switch(type)
    {
    case 0: //0-添加+左手
    {
        ui->hand1_Btn->setStyleSheet(leftHandStyle.first);
        ui->hand2_Btn->setStyleSheet(addStyle.first);
    }
        break;
    case 1: //1-添加+右手
    {
        ui->hand1_Btn->setStyleSheet(rightHandStyle.first);
        ui->hand2_Btn->setStyleSheet(addStyle.first);
    }
        break;
    case 2: //2-左手+右手
    {
        ui->hand1_Btn->setStyleSheet(leftHandStyle.first);
        ui->hand2_Btn->setStyleSheet(rightHandStyle.first);
    }
        break;
    }
}

void UserHandSelect::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(false);

    //查询当前用户信息,根据查询出的信息显示按钮
    int handType = CurrentUserData::getInstace()->getCurrentPatientMsg().hands;
    setHandBtns(handType);
}
void UserHandSelect::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

int UserHandSelect::getTrainHand()
{
    return selectedHand;
}

void UserHandSelect::slotCloseBtn()
{
    static int times = 0;
    times++;
    if(times == 2)
    {
        this->close();
        emit signalBackToEditUserPage();
        times = 0;
    }
}

//根据被选中的手获取手指的数据
bool UserHandSelect::getHandData(int selectHand,ST_HandData& st_HandData)
{
    uint32_t ID = CurrentUserData::getInstace()->getCurrentUserID();
    QString query(QString("select * from jointrangetable where ID = '%1' and hand = '%2'").arg(ID).arg(selectHand));
    if(CDatabaseInterface::getInstance()->exec(query))
    {
        int size = CDatabaseInterface::getInstance()->getValuesSize();
        if(size > 0)
        {
            QList<QVariantMap> valueMapList;
            valueMapList = CDatabaseInterface::getInstance()->getValues(0,size);

            ST_JointRange st_JointRange = variantMapToJointRange(valueMapList.at(0));
            st_HandData.hand = selectHand;

            QStringList list = st_JointRange.fingers.split('-');
            for(int i =0;i < list.size();i++)
            {
                st_HandData.fingers.append(list.at(i).toInt());
            }

            return true;
        }
    }
    else
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError()<<"Error";
    }

    return false;
}
