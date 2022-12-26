#include "edituser.h"
#include "ui_edituser.h"
#include <qvalidator.h>
#include "cdatabaseinterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QIntValidator>
#include <QSettings>
#include <QApplication>
#include <QDir>
#include "controlmaskdialog.h"
#include "currentuserdata.h"
#include <QMessageBox>
#include "cpassworddialog.h"

EditUser::EditUser(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::EditUser),
    m_handSelect(NULL),
    m_fingerSelect(NULL)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    setLineEditFormat();
    initWidgetUI();
//    writeSetting();
    m_handSelect = new UserHandSelect();
    m_fingerSelect = new UserFingerSelect();

    connect(this,SIGNAL(signalCloseEvent()),this,SLOT(slotCloseBtn()));
    connect(m_handSelect,SIGNAL(signalBackToEditUserPage()),this,SLOT(slotBackToEditUserPage()));
}

EditUser::~EditUser()
{
    if(!m_handSelect)
    {
        m_handSelect->deleteLater();
        m_handSelect = NULL;
    }
    if(!m_fingerSelect)
    {
        m_fingerSelect->deleteLater();
        m_fingerSelect = NULL;
    }
    delete ui;

}

//设置输入限制
void EditUser::setLineEditFormat()
{
    //名字
    //ID号 由系统分配，全局唯一
    //年龄、身高、体重
    QIntValidator *pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(1, 150);    //年龄
    ui->age_LineEdit->setValidator(pIntValidator);
    pIntValidator->setRange(1, 250);    //身高
    ui->height_LineEdit->setValidator(pIntValidator);
    pIntValidator->setRange(1, 300);
    ui->weight_LineEdit->setValidator(pIntValidator);
    //电话
    QRegExp reg("[0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);
    ui->patientPhone_LineEdit->setMaxLength(11);
    ui->hospitalDepartment_LineEdit->setMaxLength(10);
    ui->patientPhone_LineEdit->setValidator(pValidator);
    ui->hospitalNumber_LineEdit->setValidator(pValidator);
    ui->hospitalNumber_LineEdit->setMaxLength(20);
    //家庭住址
    ui->address_LineEdit->setMaxLength(30);
    //备注信息
    ui->remark_LineEdit->setMaxLength(100);
    //证件号
    QRegExp regNum("[a-zA-Z0-9]+$");
    pValidator->setRegExp(regNum);
    ui->healthInsuranceNumber_LineEdit->setValidator(pValidator);
    ui->healthInsuranceNumber_LineEdit->setMaxLength(20);

}

void EditUser::switchPageType(QString ID,int type)
{
    switch(type)
    {
    case 0:     //添加新用户
        addUserPage();
        setTitile(tr("新建用户"));
        break;
    case 1:     //编辑用户
        editUserPage(ID);
        setTitile(tr("编辑用户"));
        break;
    default:
        break;
    }
}

void EditUser::addUserPage()
{
    ui->delete_Btn->setVisible(false);
    ui->modify_Btn->setVisible(false);
    ui->confirm_Btn->setVisible(false);
    ui->save_Btn->setVisible(true);
    clearUserData();
    //默认为男性
    on_male_RadioButton_clicked();
    //读取当前用户ID
    readSetting();
    ui->ID_LineEdit->setText(QString::number(m_currentID+1));
}
void EditUser::editUserPage(QString ID)
{
    ui->delete_Btn->setVisible(true);
    ui->modify_Btn->setVisible(true);
    ui->confirm_Btn->setVisible(true);
    ui->save_Btn->setVisible(false);

    QString query = QString("select * from patientmessagetable where ID = '%1'").arg(ID);

    if(CDatabaseInterface::getInstance()->exec(query))
    {
        if(int count = CDatabaseInterface::getInstance()->getValuesSize() > 0)
        {
            QList<QVariantMap>valueList = CDatabaseInterface::getInstance()->getValues(0,count);
            ST_PatientMsg st_patientMsg = variantMapToPatientMsg(valueList.at(0));
            setUserData(st_patientMsg);
        }
    }
    else
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
        QMessageBox::warning(NULL,tr("警告"),tr("未找到该用户"));
    }
}

void EditUser::on_delete_Btn_clicked()
{
    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(result != 3)
        return;

    //从数据库中将该患者信息删除
    QString ID = ui->ID_LineEdit->text();
    QString dataBaseName("patientmessagetable");
    if(!CDatabaseInterface::getInstance()->deleteRowTable(dataBaseName,"ID",ID))
    {
        qDebug()<<"删除失败"<<CDatabaseInterface::getInstance()->getLastError();
        emit signalUserSelected(0);
        return;
    }
    this->close();
    //返回用户管理界面
    emit signalUserSelected(0);
    //更新用户列表
    emit signalUpdateUserList();
}

void EditUser::on_modify_Btn_clicked()
{
    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(result != 3)
        return;
    //从数据库中将该患者信息进行更新
    QString tableName("patientmessagetable");
    QVariantMap vMap = patientMsgToVariantMap(getUserData());
    if(!CDatabaseInterface::getInstance()->updateRowTable(tableName,"ID",vMap))
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
        //进入用户管理界面
        emit signalUserSelected(0);
    }
    emit signalUserSelected(0);
    emit signalUpdateUserList();
    //更新当前用户数据

    this->close();
}

void EditUser::on_confirm_Btn_clicked()
{
    //更新当前用户数据
    //将该用户添加到数据库中
    ST_PatientMsg st_PatientMsg;
    //此处需要查询数据库ID

    st_PatientMsg.ID = ui->ID_LineEdit->text().toInt();
    st_PatientMsg.name = ui->name_LineEdit->text();
    if(st_PatientMsg.name.isEmpty())
    {
        ui->tip_label->setText(tr("提示,姓名不能为空！"));
        ui->tip_label->setVisible(true);
        return;
    }
    st_PatientMsg.age = ui->age_LineEdit->text().toInt();
    if(ui->male_RadioButton->isChecked())
        st_PatientMsg.sex = 0;
    else if(ui->female_RadioButton->isChecked())
        st_PatientMsg.sex = 1;
    st_PatientMsg.height = ui->height_LineEdit->text().toInt();
    st_PatientMsg.weight = ui->weight_LineEdit->text().toInt();
    st_PatientMsg.phone = ui->patientPhone_LineEdit->text();

    //住院信息
    st_PatientMsg.hospitalDepartment = ui->hospitalDepartment_LineEdit->text();
    st_PatientMsg.hospitalNumber = ui->hospitalNumber_LineEdit->text();

    //医保账号
    st_PatientMsg.healthInsuranceType = ui->healthInsuranceType_LineEdit->text().toInt();
    st_PatientMsg.healthInsuranceNumber = ui->healthInsuranceNumber_LineEdit->text();

    st_PatientMsg.address = ui->address_LineEdit->text();
    st_PatientMsg.remarkMessage = ui->remark_LineEdit->text();

    CurrentUserData::getInstace()->setTempUserMsg(st_PatientMsg);
    this->close();
    //进入选择手信息界面

    m_handSelect->show();
}

void EditUser::on_save_Btn_clicked()
{
    //将该用户添加到数据库中
    ST_PatientMsg st_PatientMsg;
    //此处需要查询数据库ID

    st_PatientMsg.ID = ui->ID_LineEdit->text().toInt();
    st_PatientMsg.name = ui->name_LineEdit->text();
    if(st_PatientMsg.name.isEmpty())
    {
        ui->tip_label->setText(tr("提示,姓名不能为空！"));
        ui->tip_label->setVisible(true);
        return;
    }
    st_PatientMsg.age = ui->age_LineEdit->text().toInt();
    if(ui->male_RadioButton->isChecked())
        st_PatientMsg.sex = 0;
    else if(ui->female_RadioButton->isChecked())
        st_PatientMsg.sex = 1;
    st_PatientMsg.height = ui->height_LineEdit->text().toInt();
    st_PatientMsg.weight = ui->weight_LineEdit->text().toInt();
    st_PatientMsg.phone = ui->patientPhone_LineEdit->text();

    st_PatientMsg.hospitalDepartment = ui->hospitalDepartment_LineEdit->text();
    st_PatientMsg.hospitalNumber = ui->hospitalNumber_LineEdit->text();
    st_PatientMsg.healthInsuranceType = ui->healthInsuranceType_LineEdit->text().toInt();
    st_PatientMsg.healthInsuranceNumber = ui->healthInsuranceNumber_LineEdit->text();

    st_PatientMsg.address = ui->address_LineEdit->text();
    st_PatientMsg.remarkMessage = ui->remark_LineEdit->text();

    writeSetting();
    //新建用户的添加，先将数据保存到内存中
    CurrentUserData::getInstace()->setTempUserMsg(st_PatientMsg);
    this->close();

    m_fingerSelect->show();

#if 0

    QString tableName("patientmessagetable");
    QVariantMap vMap = patientMsgToVariantMap(st_PatientMsg);

    if(!CDatabaseInterface::getInstance()->insertRowTable(tableName,vMap))
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
        //进入用户管理界面
        emit signalUserSelected(0);
    }
    else
    {
        //数据插入成功
        emit signalUpdateUserList();
        //进入训练界面
        emit signalUserSelected(1);
        m_fingerSelect->show();
    }

#endif


}


//设置用户信息
void EditUser::setUserData(ST_PatientMsg st_PatientMsg)
{
    ui->ID_LineEdit->setText(QString::number(st_PatientMsg.ID));
    ui->name_LineEdit->setText(st_PatientMsg.name);
    ui->age_LineEdit->setText(QString::number(st_PatientMsg.age));

    QString pixPath;

    if(0 == st_PatientMsg.sex)
    {
        ui->male_RadioButton->setChecked(true);
        ui->female_RadioButton->setChecked(false);
        pixPath = ":/new/prefix1/source/image/male.png";
    }
    else
    {
        ui->male_RadioButton->setChecked(false);
        ui->female_RadioButton->setChecked(true);
        pixPath = ":/new/prefix1/source/image/female.png";
    }
    ui->portrait_Label->setPixmap(pixPath);

    ui->height_LineEdit->setText(QString::number(st_PatientMsg.height));
    ui->weight_LineEdit->setText(QString::number(st_PatientMsg.weight));
    ui->patientPhone_LineEdit->setText(st_PatientMsg.phone);

    ui->hospitalDepartment_LineEdit->setText(st_PatientMsg.hospitalDepartment);
    ui->hospitalNumber_LineEdit->setText(st_PatientMsg.hospitalNumber);
    ui->healthInsuranceType_LineEdit->setText(QString::number(st_PatientMsg.healthInsuranceType));
    ui->healthInsuranceNumber_LineEdit->setText(st_PatientMsg.healthInsuranceNumber);

    ui->address_LineEdit->setText(st_PatientMsg.address);
    ui->remark_LineEdit->setText(st_PatientMsg.remarkMessage);
}

ST_PatientMsg EditUser::getUserData()
{
    return st_CurrentUserData;
}

void EditUser::clearUserData()
{
    ui->ID_LineEdit->clear();
    ui->name_LineEdit->clear();
    ui->age_LineEdit->clear();

    ui->male_RadioButton->setChecked(true);
    ui->female_RadioButton->setChecked(false);

    ui->height_LineEdit->clear();
    ui->weight_LineEdit->clear();
    ui->patientPhone_LineEdit->clear();
    ui->hospitalDepartment_LineEdit->clear();
    ui->hospitalNumber_LineEdit->clear();
    ui->healthInsuranceType_LineEdit->clear();
    ui->healthInsuranceType_LineEdit->clear();
    ui->address_LineEdit->clear();
    ui->remark_LineEdit->clear();

}


void EditUser::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

void EditUser::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ui->tip_label->setVisible(false);
    ControlMaskDialog::getInstace()->setMaskDialogHide(false);
}

void EditUser::initWidgetUI()
{
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    //向四周均匀扩展阴影
    shadowEffect->setOffset(0,0);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor("#BBBBBB"));
    ui->groupBox->setGraphicsEffect(shadowEffect);

    ui->ID_LineEdit->setEnabled(false);
}

//写配置
void EditUser::writeSetting()
{
    m_currentID++;
    QString dirPath = QApplication::applicationDirPath() + "/conf/";
    QDir confdir(dirPath);
    if(!confdir.exists())
        confdir.mkdir(dirPath);
    QString confFile(dirPath + "IDconf.ini");
    QSettings iniSetting(confFile, QSettings::IniFormat);
    iniSetting.setValue("number",m_currentID);
}
//读配置
void EditUser::readSetting()
{
    QString dirPath = QApplication::applicationDirPath() + "/conf/";
    QDir confdir(dirPath);
    if(!confdir.exists())
        confdir.mkdir(dirPath);
    QString confFile(dirPath + "IDconf.ini");
    QSettings iniSetting(confFile, QSettings::IniFormat);
    m_currentID = iniSetting.value("number").toUInt();

}

void EditUser::on_male_RadioButton_clicked()
{
    QString pixPath = ":/new/prefix1/source/image/male.png";
    ui->portrait_Label->setPixmap(pixPath);
}

void EditUser::on_female_RadioButton_clicked()
{
    QString pixPath = ":/new/prefix1/source/image/female.png";
    ui->portrait_Label->setPixmap(pixPath);
}

void EditUser::slotCloseBtn()
{

    static int times = 0;
    times++;
    if(times == 2)
    {
        this->close();
        emit signalUserSelected(0);
        times = 0;
    }

}

//从选择手的页面返回到本页面
void EditUser::slotBackToEditUserPage()
{
    switchPageType(tr("编辑用户"),1);
    this->show();
}
