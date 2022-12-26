#include "controlset.h"
#include "ui_controlset.h"
#include <QButtonGroup>
#include "controlmaskdialog.h"
#include <QDebug>
#include "currentuserdata.h"
ControlSet::ControlSet(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::ControlSet)
{
    ui->setupUi(this);

    setTitile("控制设置");
    m_bucklingLevelGroup = new QButtonGroup();
    m_stretchLevelGroup = new QButtonGroup();

    m_bucklingLevelGroup->addButton(ui->bucklingLevel1_Btn,0);
    m_bucklingLevelGroup->addButton(ui->bucklingLevel2_Btn,1);
    m_bucklingLevelGroup->addButton(ui->bucklingLevel3_Btn,2);
    m_bucklingLevelGroup->setExclusive(true);
    connect(m_bucklingLevelGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotBucklingBtnGroup(int)));

    m_stretchLevelGroup->addButton(ui->stretchLevel1_Btn,0);
    m_stretchLevelGroup->addButton(ui->stretchLevel2_Btn,1);
    m_stretchLevelGroup->addButton(ui->stretchLevel3_Btn,2);
    m_stretchLevelGroup->setExclusive(false);
    connect(m_stretchLevelGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotStretchBtnGroup(int)));

    //默认为位移模式
    st_ControlParam.controlType = 1;

    st_ControlParam.bucklingLevel = 1;
    st_ControlParam.stretchLevel = 1;

    connect(this,SIGNAL(signalCloseEvent()),this,SLOT(slotCloseEvent()));

    ui->groupBox->setVisible(false);

    initWidget();
}

ControlSet::~ControlSet()
{
    if(m_bucklingLevelGroup)
    {
        m_bucklingLevelGroup->deleteLater();
        m_bucklingLevelGroup = NULL;
    }
    if(m_stretchLevelGroup)
    {
        m_stretchLevelGroup->deleteLater();
        m_stretchLevelGroup = NULL;
    }
    delete ui;

}

void ControlSet::initWidget()
{
    displacementStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/treatMode/displacementNormal.png);}\n"
                                      "QPushButton:hover{border-image: url(:/new/prefix1/source/treatMode/displacementHover.png);}");
    displacementStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/treatMode/displacementSelected.png);}");

    tendencyStyle.first = QString("QPushButton{border-image: url(:/new/prefix1/source/treatMode/tendencyNormal.png);}\n"
                                  "QPushButton:hover{border-image: url(:/new/prefix1/source/treatMode/tendencyNormalHover.png);}");
    tendencyStyle.second = QString("QPushButton{border-image: url(:/new/prefix1/source/treatMode/tendencySelected.png);}");

    ui->displacement_Btn->setStyleSheet(displacementStyle.second);

    for(int i = 0;i < 3;i++)
    {
        QString selected = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1Selected.png);}").arg(i+1);;
        levelSelectedList.append(selected);
        QString normal = QString("QPushButton{border-image: url(:/new/prefix1/source/buttons/level/level%1NoSelect.png);}\n"
                                 "QPushButton:hover{border-image: url(:/new/prefix1/source/buttons/level/level%2Hover.png);}").arg(i+1).arg(i+1);
        levelNormalList.append(normal);
    }

    ui->bucklingLevel1_Btn->setStyleSheet(levelSelectedList.at(0));
    ui->stretchLevel1_Btn->setStyleSheet(levelSelectedList.at(0));

}

void ControlSet::flushFingerState()
{
    //获取手指选中信息
    ST_HandData st_handData = CurrentUserData::getInstace()->getCurrentUserHandData();
    if(!st_handData.fingers.contains(0))
    {
        ui->thumb_RadioButton->setEnabled(false);
        ui->thumb_RadioButton->setChecked(false);
    }
    else
    {
        ui->thumb_RadioButton->setEnabled(true);
        ui->thumb_RadioButton->setChecked(true);
    }

    if(!st_handData.fingers.contains(1))
    {
        ui->foreFinger_RadioButton->setEnabled(false);
        ui->foreFinger_RadioButton->setChecked(false);
    }
    else
    {
        ui->foreFinger_RadioButton->setEnabled(true);
        ui->foreFinger_RadioButton->setChecked(true);
    }

    if(!st_handData.fingers.contains(2))
    {
        ui->middleFinger_RadioButton->setEnabled(false);
        ui->middleFinger_RadioButton->setChecked(false);
    }
    else
    {
        ui->middleFinger_RadioButton->setEnabled(true);
        ui->middleFinger_RadioButton->setChecked(true);
    }

    if(!st_handData.fingers.contains(3))
    {
        ui->ringFinger_RadioButton->setEnabled(false);
        ui->ringFinger_RadioButton->setChecked(false);
    }
    else
    {
        ui->ringFinger_RadioButton->setEnabled(true);
        ui->ringFinger_RadioButton->setChecked(true);
    }

    if(!st_handData.fingers.contains(4))
    {
        ui->littleFinger_RadioButton->setEnabled(false);
        ui->littleFinger_RadioButton->setChecked(false);
    }
    else
    {
        ui->littleFinger_RadioButton->setEnabled(true);
        ui->littleFinger_RadioButton->setChecked(true);
    }
}

//屈曲等级按钮
void ControlSet::slotBucklingBtnGroup(int btnId)
{
    st_ControlParam.bucklingLevel = btnId;
    for(int i = 0;i < m_bucklingLevelGroup->buttons().size();i++)
    {
        if(btnId == i)
        {
            m_bucklingLevelGroup->buttons().at(btnId)->setStyleSheet(levelSelectedList.at(btnId));
        }
        else
        {
            m_bucklingLevelGroup->buttons().at(i)->setStyleSheet(levelNormalList.at(i));
        }
    }
}
//伸展等级按钮
void ControlSet::slotStretchBtnGroup(int btnId)
{
    st_ControlParam.stretchLevel = btnId;
    for(int i = 0;i < m_stretchLevelGroup->buttons().size();i++)
    {
        if(btnId == i)
        {
            m_stretchLevelGroup->buttons().at(btnId)->setStyleSheet(levelSelectedList.at(btnId));
        }
        else
        {
            m_stretchLevelGroup->buttons().at(i)->setStyleSheet(levelNormalList.at(i));
        }
    }
}

void ControlSet::on_confirm_Btn_clicked()
{
    if(ui->thumb_RadioButton->isChecked())
        st_ControlParam.finger[0] = 1;
    else
        st_ControlParam.finger[0] = 0;
    if(ui->foreFinger_RadioButton->isChecked())
        st_ControlParam.finger[1] = 1;
    else
        st_ControlParam.finger[1] = 0;
    if(ui->middleFinger_RadioButton->isChecked())
        st_ControlParam.finger[2] = 1;
    else
        st_ControlParam.finger[2] = 0;
    if(ui->ringFinger_RadioButton->isChecked())
        st_ControlParam.finger[3] = 1;
    else
        st_ControlParam.finger[3] = 0;
    if(ui->littleFinger_RadioButton->isChecked())
        st_ControlParam.finger[4] = 1;
    else
        st_ControlParam.finger[4] = 0;

    this->close();
}

//力量趋势
void ControlSet::on_tendency_Btn_clicked()
{
    if(ui->tendency_Btn->styleSheet() == tendencyStyle.first)
    {
        ui->tendency_Btn->setStyleSheet(tendencyStyle.second);
        ui->displacement_Btn->setStyleSheet(displacementStyle.first);
    }
    ui->groupBox->setVisible(false);
    st_ControlParam.controlType = 1;
}

//位移控制
void ControlSet::on_displacement_Btn_clicked()
{
    if(ui->displacement_Btn->styleSheet() == displacementStyle.first)
    {
        ui->displacement_Btn->setStyleSheet(displacementStyle.second);
        ui->tendency_Btn->setStyleSheet(tendencyStyle.first);
    }
    ui->groupBox->setVisible(true);
    st_ControlParam.controlType = 2;
}

void ControlSet::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(false);
    flushFingerState();
}
void ControlSet::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}


void ControlSet::on_stretchLevel2_Btn_clicked()
{

}

ST_ControlParam ControlSet::getControlParam()
{
    return st_ControlParam;
}

void ControlSet::slotCloseEvent()
{
    this->close();
}
