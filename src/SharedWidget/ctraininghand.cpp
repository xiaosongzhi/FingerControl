#include "ctraininghand.h"
#include "ui_ctraininghand.h"
#include <QButtonGroup>
#include <QDebug>
#include "currentuserdata.h"

#define INITPOSITION 33

CTrainingHand::CTrainingHand(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTrainingHand),
    m_LButtonGroup(NULL),
    m_RButtonGroup(NULL),
    m_currentPosition(50),
    m_handType(0),
    m_scopeState(false)
{
    ui->setupUi(this);
    initWidget();
    initData();
    resize(581,585);
    for(int i =0;i < 5;i++)
        fingersEnable[i] = 1;
}

CTrainingHand::~CTrainingHand()
{
    if(m_LButtonGroup)
    {
        delete m_LButtonGroup;
        m_LButtonGroup = NULL;
    }
    if(m_RButtonGroup)
    {
        delete m_RButtonGroup;
        m_RButtonGroup = NULL;
    }
    delete ui;
}

void CTrainingHand::initWidget()
{
    m_LButtonGroup = new QButtonGroup();
    m_LButtonGroup->addButton(ui->finger0L_Btn,0);
    m_LButtonGroup->addButton(ui->finger1L_Btn,1);
    m_LButtonGroup->addButton(ui->finger2L_Btn,2);
    m_LButtonGroup->addButton(ui->finger3L_Btn,3);
    m_LButtonGroup->addButton(ui->finger4L_Btn,4);
    connect(m_LButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotLeftBtnClicked(int)));

    m_RButtonGroup = new QButtonGroup();
    m_RButtonGroup->addButton(ui->finger0R_Btn,0);
    m_RButtonGroup->addButton(ui->finger1R_Btn,1);
    m_RButtonGroup->addButton(ui->finger2R_Btn,2);
    m_RButtonGroup->addButton(ui->finger3R_Btn,3);
    m_RButtonGroup->addButton(ui->finger4R_Btn,4);
    connect(m_RButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotRightBtnClicked(int)));

    ui->finger0L_Widget->setDirection(0);
    ui->finger1L_Widget->setDirection(1);
    ui->finger2L_Widget->setDirection(1);
    ui->finger3L_Widget->setDirection(1);
    ui->finger4L_Widget->setDirection(1);

    m_LProcessBar.append(ui->finger0L_Widget);
    m_LProcessBar.append(ui->finger1L_Widget);
    m_LProcessBar.append(ui->finger2L_Widget);
    m_LProcessBar.append(ui->finger3L_Widget);
    m_LProcessBar.append(ui->finger4L_Widget);

    ui->finger0R_Widget->setDirection(0);
    ui->finger1R_Widget->setDirection(1);
    ui->finger2R_Widget->setDirection(1);
    ui->finger3R_Widget->setDirection(1);
    ui->finger4R_Widget->setDirection(1);

    m_RProcessBar.append(ui->finger0R_Widget);
    m_RProcessBar.append(ui->finger1R_Widget);
    m_RProcessBar.append(ui->finger2R_Widget);
    m_RProcessBar.append(ui->finger3R_Widget);
    m_RProcessBar.append(ui->finger4R_Widget);
}

void CTrainingHand::setMode(E_TRAINMODE mode)
{
    switch(mode)
    {
    case TRAINMODE_PASSIVE_E:       //被动和助力都设置成该状态
    case TRAINMODE_BOOST_E:
    {
        ui->finger0L_Widget->setScopeRectVisible(true);
        ui->finger1L_Widget->setScopeRectVisible(true);
        ui->finger2L_Widget->setScopeRectVisible(true);
        ui->finger3L_Widget->setScopeRectVisible(true);
        ui->finger4L_Widget->setScopeRectVisible(true);
        ui->finger0L_Widget->switchPassive(true);
        ui->finger1L_Widget->switchPassive(true);
        ui->finger2L_Widget->switchPassive(true);
        ui->finger3L_Widget->switchPassive(true);
        ui->finger4L_Widget->switchPassive(true);

        ui->finger0R_Widget->setScopeRectVisible(true);
        ui->finger1R_Widget->setScopeRectVisible(true);
        ui->finger2R_Widget->setScopeRectVisible(true);
        ui->finger3R_Widget->setScopeRectVisible(true);
        ui->finger4R_Widget->setScopeRectVisible(true);
        ui->finger0R_Widget->switchPassive(true);
        ui->finger1R_Widget->switchPassive(true);
        ui->finger2R_Widget->switchPassive(true);
        ui->finger3R_Widget->switchPassive(true);
        ui->finger4R_Widget->switchPassive(true);
    }
        break;
    case TRAINMODE_ACTIVE_E:
        break;
    case TRAINMODE_MEMORY_E:
        break;
    }
}

//初始化数据
void CTrainingHand::initData()
{
    //手指状态数据填充
    QList<bool> fingerState;
    fingerState << false << false <<false << false <<false;
    m_mapHandFingerState.insert(0,fingerState);
    m_mapHandFingerState.insert(1,fingerState);

    m_mapFingersRange.clear();
    for(int i = 0;i < 5;i++)
    {
        QPair<int,int> initPosition;
        initPosition.first = INITPOSITION;
        initPosition.second = INITPOSITION;
        m_mapFingersRange.insert(i,initPosition);
    }
}

void CTrainingHand::slotRightBtnClicked(int btnId)
{
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    QAbstractButton * selectBtn = m_RButtonGroup->button(btnId);
    //关闭该手指
    if(selectBtn->styleSheet() == turnOn)
    {
        emit signalFingerStateChanged(btnId,0);
        selectBtn->setStyleSheet(turnOff);
        foreach(CProcessBar *tempBar,m_RProcessBar)
        {
            if(tempBar->objectName().contains(QString::number(btnId)))
            {
                tempBar->setFingerEnable(false);
                return;
            }
        }
    }
    //打开该手指
    else if(selectBtn->styleSheet() == turnOff)
    {
        emit signalFingerStateChanged(btnId,1);
        selectBtn->setStyleSheet(turnOn);
        foreach(CProcessBar *tempBar,m_RProcessBar)
        {
            if(tempBar->objectName().contains(QString::number(btnId)))
            {
                tempBar->setFingerEnable(true);
                return;
            }
        }
    }
}

void CTrainingHand::slotLeftBtnClicked(int btnId)
{
    QAbstractButton * selectBtn = m_LButtonGroup->button(btnId);
    QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
    QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
    //关闭该手指
    if(selectBtn->styleSheet() == turnOn)
    {
        emit signalFingerStateChanged(btnId,0);
        selectBtn->setStyleSheet(turnOff);
        foreach(CProcessBar *tempBar,m_LProcessBar)
        {
            if(tempBar->objectName().contains(QString::number(btnId)))
            {
                tempBar->setFingerEnable(false);
                return;
            }
        }
    }
    //打开该手指
    else if(selectBtn->styleSheet() == turnOff)
    {
        emit signalFingerStateChanged(btnId,1);
        selectBtn->setStyleSheet(turnOn);
        foreach(CProcessBar *tempBar,m_LProcessBar)
        {

            if(tempBar->objectName().contains(QString::number(btnId)))
            {
                tempBar->setFingerEnable(true);
                return;
            }
        }
    }
}

//待完成该函数 使能状态改变(已完成)
void CTrainingHand::setHandFingerEnable(int handType,int fingerIndex,bool isEnable)
{
    if(isEnable)
        fingersEnable[fingerIndex] = 1;
    else
        fingersEnable[fingerIndex] = 0;
    m_mapHandFingerState[handType].replace(fingerIndex,isEnable);
    switch(handType)
    {
    case 0://左手
    {
        QAbstractButton * selectBtn = m_LButtonGroup->button(fingerIndex);
        QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
        QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
        //关闭该手指
        if(selectBtn->styleSheet() == turnOn && (!isEnable))
        {
            selectBtn->setStyleSheet(turnOff);
            foreach(CProcessBar *tempBar,m_LProcessBar)
            {
                if(tempBar->objectName().contains(QString::number(fingerIndex)))
                {
                    tempBar->setFingerEnable(false);
                    return;
                }
            }
        }
        //打开该手指
        else if(selectBtn->styleSheet() == turnOff && isEnable)
        {
            selectBtn->setStyleSheet(turnOn);
            foreach(CProcessBar *tempBar,m_LProcessBar)
            {

                if(tempBar->objectName().contains(QString::number(fingerIndex)))
                {
                    tempBar->setFingerEnable(true);
                    return;
                }
            }
        }
    }
        break;
    case 1://右手
    {
        QAbstractButton * selectBtn = m_RButtonGroup->button(fingerIndex);
        QString turnOn("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOn.png);");
        QString turnOff("border-image: url(:/new/prefix1/source/buttons/generalButton/turnOff.png);");
        //关闭该手指
        if(selectBtn->styleSheet() == turnOn && (!isEnable))
        {
            selectBtn->setStyleSheet(turnOff);
            foreach(CProcessBar *tempBar,m_RProcessBar)
            {
                if(tempBar->objectName().contains(QString::number(fingerIndex)))
                {
                    tempBar->setFingerEnable(false);
                    return;
                }
            }
        }
        //打开该手指
        else if(selectBtn->styleSheet() == turnOff && isEnable)
        {
            selectBtn->setStyleSheet(turnOn);
            foreach(CProcessBar *tempBar,m_RProcessBar)
            {

                if(tempBar->objectName().contains(QString::number(fingerIndex)))
                {
                    tempBar->setFingerEnable(true);
                    return;
                }
            }
        }
    }
        break;
    }
}

void CTrainingHand::switchHand(int handType)
{
    m_handType = handType;
    switch(handType)
    {
    case 0:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    }
}

//获取单个手指的范围
void CTrainingHand::setFingerValueMargin(int fingerIndex,int value)
{
    if(m_mapFingersRange[fingerIndex].first > value)
        m_mapFingersRange[fingerIndex].first = value;
    if(m_mapFingersRange[fingerIndex].second < value)
        m_mapFingersRange[fingerIndex].second = value;
}

//设置各个手指的值
void CTrainingHand::setHandFingerValue(int handType,int fingerIndex,int value)
{
    static int temp[5] = {-1,-1,-1,-1,-1};
    if((fingerIndex < 5) && (temp[fingerIndex] != value))
    {
        int minValue = 0;
        int maxValue = 0;
        temp[fingerIndex] = value;

        //设置各个手指的活动范围

            setFingerValueMargin(fingerIndex,value);
        int m_handType = CurrentUserData::getInstace()->getCurrentUserHandData().hand;
        //当前手
        if(handType == m_handType)
        {   //当前手指
            if(m_mapHandFingerState[m_handType].at(fingerIndex))
            {
                QString fingerName("");
                QObjectList objectList;
                switch(m_handType)
                {
                case 0:
                    fingerName = QString("finger%1%2_Widget").arg(fingerIndex).arg('L');
                    objectList = ui->left_GroupBox->children();
                    break;
                case 1:
                    fingerName = QString("finger%1%2_Widget").arg(fingerIndex).arg('R');
                    objectList = ui->right_GroupBox->children();
                    break;
                }

                for(int i = 0;i < objectList.size();i++)
                {
                    if(fingerName == objectList.at(i)->objectName())
                    {

                        CProcessBar *probar = static_cast<CProcessBar *>(objectList.at(i));
                        minValue = m_mapFingersRange[fingerIndex].first;
                        maxValue = m_mapFingersRange[fingerIndex].second;
                        probar->setMarkLine(minValue);
                        if(m_scopeState)
                            probar->setMoveRectMinMargin();
                        probar->setMarkLine(maxValue);
                        if(m_scopeState)
                            probar->setMoveRectMaxMargin();
                        probar->setMarkLine(value);
                        //获取当前手指的位置
                        m_currentPosition = value;
                    }
                }
            }
        }
    }
}

//重置各个手指
void CTrainingHand::resetHandFingerValue(int handType,int fingerIndex)
{
    m_mapFingersRange[fingerIndex].first = m_currentPosition;
    m_mapFingersRange[fingerIndex].second = m_currentPosition;

    QString fingerName("");
    QObjectList objectList;
    switch(handType)
    {
    case 0:
        fingerName = QString("finger%1%2_Widget").arg(fingerIndex).arg('L');
        objectList = ui->left_GroupBox->children();
        break;
    case 1:
        fingerName = QString("finger%1%2_Widget").arg(fingerIndex).arg('R');
        objectList = ui->right_GroupBox->children();
        break;
    }

    for(int i = 0;i < objectList.size();i++)
    {
        if(fingerName == objectList.at(i)->objectName())
        {
            CProcessBar *probar = static_cast<CProcessBar *>(objectList.at(i));
            probar->resetFingerValue(m_currentPosition);
        }
    }
}

QMap<int,QPair<int,int>> CTrainingHand::getFingersRange()
{
    return m_mapFingersRange;
}


//设置基础活动范围，该活动范围从关节活动度界面获取 2021.9.14
void CTrainingHand::setBaseMoveScope(const QMap<int,QPair<int,int>>& fingersRange)
{
    switch(m_handType)
    {
    case LeftHand_E:
    {
        if(fingersEnable[0] && fingersRange.contains(0))
            ui->finger0L_Widget->setBaseScopeRect(fingersRange[0].first,fingersRange[0].second);
        if(fingersEnable[1] && fingersRange.contains(1))
            ui->finger1L_Widget->setBaseScopeRect(fingersRange[1].first,fingersRange[1].second);
        if(fingersEnable[2] && fingersRange.contains(2))
            ui->finger2L_Widget->setBaseScopeRect(fingersRange[2].first,fingersRange[2].second);
        if(fingersEnable[3] && fingersRange.contains(3))
            ui->finger3L_Widget->setBaseScopeRect(fingersRange[3].first,fingersRange[3].second);
        if(fingersEnable[4] && fingersRange.contains(4))
            ui->finger4L_Widget->setBaseScopeRect(fingersRange[4].first,fingersRange[4].second);
    }
        break;
    case RightHand_E:
    {
        if(fingersEnable[0] && fingersRange.contains(0))
            ui->finger0R_Widget->setBaseScopeRect(fingersRange[0].first,fingersRange[0].second);
        if(fingersEnable[1] && fingersRange.contains(1))
            ui->finger1R_Widget->setBaseScopeRect(fingersRange[1].first,fingersRange[1].second);
        if(fingersEnable[2] && fingersRange.contains(2))
            ui->finger2R_Widget->setBaseScopeRect(fingersRange[2].first,fingersRange[2].second);
        if(fingersEnable[3] && fingersRange.contains(3))
            ui->finger3R_Widget->setBaseScopeRect(fingersRange[3].first,fingersRange[3].second);
        if(fingersEnable[4] && fingersRange.contains(4))
            ui->finger4R_Widget->setBaseScopeRect(fingersRange[4].first,fingersRange[4].second);
    }
        break;
    }
}

//设置各个关节活动的实际活动范围 2021.9.14
void CTrainingHand::setRealMoveScope(const QMap<int,QPair<int,int>>& fingersRange)
{
    switch(m_handType)
    {
    case LeftHand_E:
    {
        if(fingersEnable[0] && fingersRange.contains(0))
        {
            ui->finger0L_Widget->setMoveScopeRect(fingersRange[0].first,fingersRange[0].second);
        }
        if(fingersEnable[1] && fingersRange.contains(1))
            ui->finger1L_Widget->setMoveScopeRect(fingersRange[1].first,fingersRange[1].second);
        if(fingersEnable[2] && fingersRange.contains(2))
            ui->finger2L_Widget->setMoveScopeRect(fingersRange[2].first,fingersRange[2].second);
        if(fingersEnable[3] && fingersRange.contains(3))
            ui->finger3L_Widget->setMoveScopeRect(fingersRange[3].first,fingersRange[3].second);
        if(fingersEnable[4] && fingersRange.contains(4))
            ui->finger4L_Widget->setMoveScopeRect(fingersRange[4].first,fingersRange[4].second);
    }
        break;
    case RightHand_E:
    {
        if(fingersEnable[0] && fingersRange.contains(0))
            ui->finger0R_Widget->setMoveScopeRect(fingersRange[0].first,fingersRange[0].second);
        if(fingersEnable[1] && fingersRange.contains(1))
            ui->finger1R_Widget->setMoveScopeRect(fingersRange[1].first,fingersRange[1].second);
        if(fingersEnable[2] && fingersRange.contains(2))
            ui->finger2R_Widget->setMoveScopeRect(fingersRange[2].first,fingersRange[2].second);
        if(fingersEnable[3] && fingersRange.contains(3))
            ui->finger3R_Widget->setMoveScopeRect(fingersRange[3].first,fingersRange[3].second);
        if(fingersEnable[4] && fingersRange.contains(4))
            ui->finger4R_Widget->setMoveScopeRect(fingersRange[4].first,fingersRange[4].second);
    }
        break;
    }
}

void CTrainingHand::switchScope(bool ok)
{
    m_scopeState = ok;
}
