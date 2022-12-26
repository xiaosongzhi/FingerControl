#include "cpassworddialog.h"
#include "ui_cpassworddialog.h"
#include <QDebug>
#include <QIntValidator>
#include "controlmaskdialog.h"
#include <QPainter>
CPasswordDialog::CPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPasswordDialog)
{
    ui->setupUi(this);
//    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    ui->lineEdit->setMaxLength(8);
    //设置为模态对话框
    setModal(true);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit->setPlaceholderText(tr("请输入密码"));
//    ui->lineEdit->setInputMask("0-0-0-0-0-0-0-0");

    setLineEditInput();

    ui->confirm_Btn->setDefault(true);

    connect(this,&CPasswordDialog::signalResult,this,&CPasswordDialog::done);
}

CPasswordDialog::~CPasswordDialog()
{
    delete ui;
}

void CPasswordDialog::on_confirm_Btn_clicked()
{
    QString password = ui->lineEdit->text();

    if("11111111" == password)
    {
        emit signalResult(3);
        this->close();
    }
    else
    {
        ui->tip_Label->setText(tr("密码错误,请重试"));
    }

}

void CPasswordDialog::on_cancel_Btn_clicked()
{
    this->close();
//    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

void CPasswordDialog::on_close_Btn_clicked()
{
    this->close();
}

void CPasswordDialog::setLineEditInput()
{
    // 整形 范围：[1, 99]
    QIntValidator *pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(1, 99);

    // 浮点型 范围：[-360, 360] 精度：小数点后2位
    QDoubleValidator *pDoubleValidator = new QDoubleValidator(this);
    pDoubleValidator->setRange(0, 100);
    pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    pDoubleValidator->setDecimals(2);

    // 字符和数字
    QRegExp reg("[a-zA-Z0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);

    ui->lineEdit->setValidator(pValidator);
}

void CPasswordDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
//    ControlMaskDialog::getInstace()->setMaskDialogHide(false);
}

void CPasswordDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
//    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

void CPasswordDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,100));

}
