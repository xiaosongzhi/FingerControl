#include "userfingerselect.h"
#include "ui_userfingerselect.h"
#include "controlmaskdialog.h"
#include "userdataformate.h"
#include "currentuserdata.h"
#include "mainwindowpagecontrol.h"
#include <QDebug>
#include <QMessageBox>
UserFingerSelect::UserFingerSelect(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::UserFingerSelect),
    m_radioButtonGroup(NULL),
    m_handType(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    ui->left_RadioButton->setChecked(true);

    m_radioButtonGroup = new QButtonGroup();
    m_radioButtonGroup->addButton(ui->finger0_RadioButton,0);
    m_radioButtonGroup->addButton(ui->finger1_RadioButton,1);
    m_radioButtonGroup->addButton(ui->finger2_RadioButton,2);
    m_radioButtonGroup->addButton(ui->finger3_RadioButton,3);
    m_radioButtonGroup->addButton(ui->finger4_RadioButton,4);
    m_radioButtonGroup->setExclusive(false);
    connect(m_radioButtonGroup,SIGNAL(buttonToggled(int,bool)),this,SLOT(slotButtonToogled(int,bool)));
    foreach(QAbstractButton *button,m_radioButtonGroup->buttons())
    {
        button->setChecked(true);
    }

}

UserFingerSelect::~UserFingerSelect()
{
    delete ui;
}

void UserFingerSelect::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    ControlMaskDialog::getInstace()->setMaskDialogHide(false);

}
void UserFingerSelect::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    //关闭该界面时需要回到上一界面
}

void UserFingerSelect::on_confirm_Btn_clicked()
{
    ST_HandData st_HandData;
    if(ui->left_RadioButton->isChecked())
        st_HandData.hand = 0;
    else
        st_HandData.hand = 1;
    if(ui->finger0_RadioButton->isChecked())
    {
        st_HandData.fingers.append(0);
        st_HandData.fingerSelected[0] = 1;
    }
    else
        st_HandData.fingerSelected[0] = 0;
    if(ui->finger1_RadioButton->isChecked())
    {
        st_HandData.fingers.append(1);
        st_HandData.fingerSelected[1] = 1;
    }
    else
        st_HandData.fingerSelected[1] = 0;

    if(ui->finger2_RadioButton->isChecked())
    {
        st_HandData.fingers.append(2);
        st_HandData.fingerSelected[2] = 1;
    }
    else
        st_HandData.fingerSelected[2] = 0;

    if(ui->finger3_RadioButton->isChecked())
    {
        st_HandData.fingers.append(3);
        st_HandData.fingerSelected[3] = 1;
    }
    else
        st_HandData.fingerSelected[3] = 0;

    if(ui->finger4_RadioButton->isChecked())
    {
        st_HandData.fingers.append(4);
        st_HandData.fingerSelected[4] = 1;
    }
    else
        st_HandData.fingerSelected[4] = 0;

    if(st_HandData.fingers.isEmpty())
    {

        //弹出提示框
        return;
    }

    CurrentUserData::getInstace()->setTempUserHandData(st_HandData);
    this->close();

    MainWindowPageControl::getInstance()->setCurrentPage(JointMotionScopePage_E);
}

void UserFingerSelect::on_left_RadioButton_clicked(bool checked)
{
    if(checked)
    {
        m_handType = 0;
        ui->widget->setHandType(0);
    }
}

void UserFingerSelect::on_right_RadioButton_clicked(bool checked)
{
    if(checked)
    {
        m_handType = 1;
        ui->widget->setHandType(1);
    }
}

void UserFingerSelect::setLeftRightHand(int handType)
{
    m_handType = handType;
    ui->widget->setHandType(handType);
    switch(handType)
    {
    case 0:
        ui->left_RadioButton->setChecked(true);
        break;
    case 1:
        ui->right_RadioButton->setChecked(true);
        break;
    }
}

void UserFingerSelect::slotButtonToogled(int id,bool checked)
{
    if(checked)
        ui->widget->setFingerState(id,NORMAL_E);
    else
        ui->widget->setFingerState(id,DisableGray_E);
}
