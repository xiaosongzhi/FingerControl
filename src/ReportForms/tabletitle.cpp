#include "tabletitle.h"
#include "ui_tabletitle.h"
#include <QDateTime>
TableTitle::TableTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableTitle)
{
    ui->setupUi(this);
}

TableTitle::~TableTitle()
{
    delete ui;
}

//清除报表头
void TableTitle::clearTitle()
{
    ui->age_Label->clear();
    ui->dateTime_Label->clear();
    ui->department_Label->clear();
    ui->name_Label->clear();
    ui->phoneNum_Label->clear();
    ui->sex_Label->clear();
}
//设置报表头信息
void TableTitle::setTitleMessage(const ST_PatientMsg &st_patientMsg)
{
    ui->name_Label->setText(st_patientMsg.name);
    ui->phoneNum_Label->setText(st_patientMsg.phone);
    if(0 == st_patientMsg.sex)
        ui->sex_Label->setText(tr("男"));
    else if(1 == st_patientMsg.sex)
        ui->sex_Label->setText(tr("女"));
    if(!st_patientMsg.hospitalDepartment.isEmpty())
        ui->department_Label->setText(st_patientMsg.hospitalDepartment);
    else
        ui->department_Label->setText(tr("康复科"));
    ui->age_Label->setText(QString::number(st_patientMsg.age));
    ui->dateTime_Label->setText(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
}

