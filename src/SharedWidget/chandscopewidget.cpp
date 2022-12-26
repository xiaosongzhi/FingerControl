#include "chandscopewidget.h"
#include "ui_chandscopewidget.h"
#include <QDebug>
#include <QButtonGroup>
#include <QPushButton>
#define INITPOSITION 33

CHandScopeWidget::CHandScopeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHandScopeWidget),
    m_handType(0),
    m_plusGroup(NULL),
    m_minusGroup(NULL),
    currentPosition(50)
{
    ui->setupUi(this);

    initWidget();

    resize(434,563);

    initData();
}

CHandScopeWidget::~CHandScopeWidget()
{
    if(m_plusGroup)
    {
        m_plusGroup->deleteLater();
        m_plusGroup = NULL;
    }
    if(m_minusGroup)
    {
        m_plusGroup->deleteLater();
        m_plusGroup = NULL;
    }
    delete ui;
}

void CHandScopeWidget::initWidget()
{
    ui->finger0L_Widget->setDirection(0);
    ui->finger1L_Widget->setDirection(1);
    ui->finger2L_Widget->setDirection(1);
    ui->finger3L_Widget->setDirection(1);
    ui->finger4L_Widget->setDirection(1);
    ui->finger0R_Widget->setDirection(0);
    ui->finger1R_Widget->setDirection(1);
    ui->finger2R_Widget->setDirection(1);
    ui->finger3R_Widget->setDirection(1);
    ui->finger4R_Widget->setDirection(1);

    //左手按钮
    foreach(QObject* object,ui->left_GroupBox->children())
    {
        if(object->objectName().contains("Btn"))
        {
            leftHandFingerBtnList.append(static_cast<QPushButton*>(object));
        }
    }
    foreach(QObject* object,ui->right_GroupBox->children())
    {
        if(object->objectName().contains("Btn"))
        {
            rightHandFingerBtnList.append(static_cast<QPushButton*>(object));
        }
    }

    m_minusGroup = new QButtonGroup();
    //左手 down
    m_minusGroup->addButton(ui->LLeft0_Btn,0);
    m_minusGroup->addButton(ui->LDown1_Btn,1);
    m_minusGroup->addButton(ui->LDown2_Btn,2);
    m_minusGroup->addButton(ui->LDown3_Btn,3);
    m_minusGroup->addButton(ui->LDown4_Btn,4);

    //右手 down
    m_minusGroup->addButton(ui->RLeft0_Btn,5);
    m_minusGroup->addButton(ui->RDown1_Btn,6);
    m_minusGroup->addButton(ui->RDown2_Btn,7);
    m_minusGroup->addButton(ui->RDown3_Btn,8);
    m_minusGroup->addButton(ui->RDown4_Btn,9);

    connect(m_minusGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotMinusBtnGroup(int)));

    m_plusGroup = new QButtonGroup();
    //左手up
    m_plusGroup->addButton(ui->LRight0_Btn,0);
    m_plusGroup->addButton(ui->LUp1_Btn,1);
    m_plusGroup->addButton(ui->LUp2_Btn,2);
    m_plusGroup->addButton(ui->LUp3_Btn,3);
    m_plusGroup->addButton(ui->LUp4_Btn,4);
    //右手up
    m_plusGroup->addButton(ui->RRight0_Btn,5);
    m_plusGroup->addButton(ui->RUp1_Btn,6);
    m_plusGroup->addButton(ui->RUp2_Btn,7);
    m_plusGroup->addButton(ui->RUp3_Btn,8);
    m_plusGroup->addButton(ui->RUp4_Btn,9);

    connect(m_plusGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotPlusBtnGroup(int)));

}

void CHandScopeWidget::switchHand(int type)//0-左手 1-右手
{
    m_handType = type;
    ui->stackedWidget->setCurrentIndex(type);
}

//该函数需要记录十个手指的数据范围
void CHandScopeWidget::getFingerValueMargin(int fingerIndex,int value)
{
    if(m_tempFingerRange[fingerIndex].first > value)
        m_tempFingerRange[fingerIndex].first = value;
    if(m_tempFingerRange[fingerIndex].second < value)
        m_tempFingerRange[fingerIndex].second = value;
}

void CHandScopeWidget::setHandFingerValue(int handType,int fingerIndex,int value)
{
    static int temp[5] = {-1,-1,-1,-1,-1};
    if((fingerIndex < 5) && (temp[fingerIndex] != value))
    {
        int minValue = 0;
        int maxValue = 0;
        temp[fingerIndex] = value;

        getFingerValueMargin(fingerIndex,value);
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
                        minValue = m_tempFingerRange[fingerIndex].first;
                        maxValue = m_tempFingerRange[fingerIndex].second;
                        probar->setMark(minValue);
                        probar->setMoveRectMinMargin();
                        probar->setMark(maxValue);
                        probar->setMoveRectMaxMargin();
                        probar->setMark(value);
                        //获取当前手指的位置
//                        currentPosition = value;
                        //测试使用
                        fingersCurrentPosition[fingerIndex] = value;
                    }
                }
            }
        }
    }
}

//重置各个手指
void CHandScopeWidget::resetHandFingerValue(int handType,int fingerIndex)
{
    //初始化手指位置，具体位置为INITPOSITION
//    m_tempFingerRange[fingerIndex].first = currentPosition;
//    m_tempFingerRange[fingerIndex].second = currentPosition;
//    for(int i =0;i < 5;i++)
//        qDebug()<<fingersCurrentPosition[i];

    m_tempFingerRange[fingerIndex].first = fingersCurrentPosition[fingerIndex];
    m_tempFingerRange[fingerIndex].second = fingersCurrentPosition[fingerIndex];

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
            //测试使用
//            probar->resetFingerValue(currentPosition);

            probar->resetFingerValue(fingersCurrentPosition[fingerIndex]);
        }
    }
}
//禁用或者使能手指
void CHandScopeWidget::setHandFingerEnable(int handType,int fingerIndex,bool isEnable)
{
    //为防止出界
    if(m_mapHandFingerState[handType].size() > fingerIndex)
        m_mapHandFingerState[handType][fingerIndex] = isEnable;
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
            probar->setFingerEnable(isEnable);
        }
    }
    //设置箭头按钮的使能与否
    setFingerBtnEnable(handType,fingerIndex,isEnable);

}

void CHandScopeWidget::setFingerBtnEnable(int handType,int fingerIndex,bool isEnable)
{
    //设置箭头按钮的使能与否
    QString downEnableStyle("border-image: url(:/new/prefix1/source/image/downNormal.png);");
    QString downDisableStyle("border-image: url(:/new/prefix1/source/image/downDisable.png);");
    QString upEnableStyle("border-image: url(:/new/prefix1/source/image/upNormal.png);");
    QString upDisableStyle("border-image: url(:/new/prefix1/source/image/upDisable.png);");
    QString leftEnableStyle("border-image: url(:/new/prefix1/source/image/leftNormal.png);");
    QString leftDisableStyle("border-image: url(:/new/prefix1/source/image/leftDisabled.png);");
    QString rightEnableStyle("border-image: url(:/new/prefix1/source/image/rightNormal.png);");
    QString rightDisableStyle("border-image: url(:/new/prefix1/source/image/rightDisabled.png);");

    QString index = QString::number(fingerIndex);
    switch(handType)
    {
    //选择手
    case 0:
    {
        for(int i = 0;i < leftHandFingerBtnList.size();i++)
        {
            QPushButton *btn = leftHandFingerBtnList.at(i);
#if 1       //选择手指
            if(btn->objectName().contains(index))
            {
                if(0 == fingerIndex)//左手拇指
                {
                    //LLeft0_Btn LRight0_Btn
                    //左手拇指左侧
                    if(btn->objectName().contains("Left"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(leftEnableStyle);
                        else
                            btn->setStyleSheet(leftDisableStyle);

                    }
                    //左手拇指右侧
                    else if(btn->objectName().contains("Right"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(rightEnableStyle);
                        else
                            btn->setStyleSheet(rightDisableStyle);
                    }
                }
                else //左手四指
                {
                    //左手四指上侧
                    if(btn->objectName().contains("Up"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(upEnableStyle);
                        else
                            btn->setStyleSheet(upDisableStyle);
                    }
                    //左手四指下侧
                    else if(btn->objectName().contains("Down"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(downEnableStyle);
                        else
                            btn->setStyleSheet(downDisableStyle);
                    }
                }
                //设置按钮的使能与否
                btn->setEnabled(isEnable);
                //break;
            }
#endif
        }
    }
        break;
    case 1:
    {
        for(int i = 0;i < rightHandFingerBtnList.size();i++)
        {
            QPushButton *btn = rightHandFingerBtnList.at(i);

            if(btn->objectName().contains(index))
            {
                if(0 == fingerIndex)//右手拇指
                {
                    //右手拇指左侧
                    if(btn->objectName().contains("Left"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(leftEnableStyle);
                        else
                            btn->setStyleSheet(leftDisableStyle);
                    }
                    //右手拇指右侧
                    else if(btn->objectName().contains("Right"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(rightEnableStyle);
                        else
                            btn->setStyleSheet(rightDisableStyle);
                    }
                }
                else //右手四指
                {
                    //右手四指上侧
                    if(btn->objectName().contains("Up"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(upEnableStyle);
                        else
                            btn->setStyleSheet(upDisableStyle);
                    }
                    //右手四指下侧
                    else if(btn->objectName().contains("Down"))
                    {
                        if(isEnable)
                            btn->setStyleSheet(downEnableStyle);
                        else
                            btn->setStyleSheet(downDisableStyle);
                    }
                }
                //设置按钮的使能与否
                btn->setEnabled(isEnable);
            }
        }
    }
        break;
    }
}

QMap<int,QPair<int,int>> CHandScopeWidget::getFingersRange()
{
    //此处需要区分左右手
    //拇指
    if(0 == m_handType)
    {
        m_mapFingersRange[0].first = 100 - m_tempFingerRange[0].first;
        m_mapFingersRange[0].second = 100 - m_tempFingerRange[0].second;
    }
    else if(1 == m_handType)
    {
        m_mapFingersRange[0].first = m_tempFingerRange[0].first;
        m_mapFingersRange[0].second = m_tempFingerRange[0].second;
    }

    //四指
    m_mapFingersRange[1].first =  m_tempFingerRange[1].second;
    m_mapFingersRange[1].second =  m_tempFingerRange[1].first;
    m_mapFingersRange[2].first = m_tempFingerRange[2].second;
    m_mapFingersRange[2].second = m_tempFingerRange[2].first;
    m_mapFingersRange[3].first = m_tempFingerRange[3].second;
    m_mapFingersRange[3].second = m_tempFingerRange[3].first;
    m_mapFingersRange[4].first = m_tempFingerRange[4].second;
    m_mapFingersRange[4].second = m_tempFingerRange[4].first;
    return m_mapFingersRange;
}

//获取相对活动度
QMap<int,QPair<int,int>> CHandScopeWidget::getOppositeRange()
{
    return m_tempFingerRange;
}

void CHandScopeWidget::slotMinusBtnGroup(int btnId)
{
    //向下运动
    emit singnalFingerValueChanged(btnId/5,btnId%5,-1);
}

void CHandScopeWidget::slotPlusBtnGroup(int btnId)
{
    //向上运动
    emit singnalFingerValueChanged(btnId/5,btnId%5,1);
}

//初始化数据
void CHandScopeWidget::initData()
{
    QList<bool> fingerState;
    fingerState << false << false <<false << false <<false;
    m_mapHandFingerState.insert(0,fingerState);
    m_mapHandFingerState.insert(1,fingerState);
    m_tempFingerRange.clear();
    m_mapFingersRange.clear();
    for(int i = 0;i < 5;i++)
    {
        QPair<int,int> initPosition;
        initPosition.first = INITPOSITION;
        initPosition.second = INITPOSITION;
        m_tempFingerRange.insert(i,initPosition);
        m_mapFingersRange.insert(i,initPosition);
        fingersCurrentPosition[i] = 50;
    }
}
