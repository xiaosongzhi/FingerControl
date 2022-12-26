#include "machinecheckdialog.h"
#include "ui_machinecheckdialog.h"
#include "ccommunicateapi.h"
MachineCheckDialog::MachineCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachineCheckDialog),
    m_timer(NULL)
{
    ui->setupUi(this);
    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateProcess()));
    m_timer->setInterval(100);
}

MachineCheckDialog::~MachineCheckDialog()
{
    delete ui;
}

void MachineCheckDialog::updateProcess(int value)
{
    if(value == 100)
        ui->label->setText(tr("校准完成"));
    ui->progressBar->setValue(value*3);
}

void MachineCheckDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    m_timer->start();

}
void MachineCheckDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    m_timer->stop();
}

void MachineCheckDialog::slotUpdateProcess()
{
    static int value = 0;
    value++;
    ui->progressBar->setValue(value);
    if(value == 100)
        value = 0;
}
