#include "warningdialog.h"
#include "ui_warningdialog.h"
#include <QPixmap>
#include <QApplication>
#include "controlmaskdialog.h"
#include <QTimer>
#include <QFile>
#include "logger.h"
#include <QDebug>
#include <QPainter>
#pragma execution_character_set("utf-8")
//LOG4QT_DECLARE_STATIC_LOGGER(logger, WarningDialog)
WarningDialog::WarningDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningDialog),
    m_closeTimer(NULL),
    m_type(EMERGENCY_E),
    isConfirmPressed(false)
{
    ui->setupUi(this);
//    resize(1920,1080);
//    QPalette palette;
//    palette.setColor(QPalette::Background,QColor(0x00,0x00,0x00,0xff));
//    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
    setAttribute(Qt::WA_TranslucentBackground,true);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    setWindowOpacity(0.2);

    setAttribute(Qt::WA_QuitOnClose,false);
    m_closeTimer = new QTimer();
    connect(m_closeTimer,&QTimer::timeout,this,&WarningDialog::slotCloseEvent);
    m_closeTimer->setSingleShot(true);
    loadStyleSheetFile();
}

WarningDialog::~WarningDialog()
{
    delete ui;
    if(m_closeTimer)
    {
        delete  m_closeTimer;
        m_closeTimer = NULL;
    }
}

void WarningDialog::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/waitingDialog.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        qDebug("waitingDialog.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void WarningDialog::setImageAndText(QString path,QString text)
{
    ui->text_Label->setText(text);
    QPixmap pixmap(path);
    ui->image_Label->setPixmap(pixmap);
}

void WarningDialog::setWarningType(E_WarningType type)
{
    m_type = type;
    switch(type)
    {
    case EMERGENCY_E:
        showEmergencyDialog();
        break;
    case SPASM_E:
        showSpasmDialog();
        break;
    case CONNECTLOST_E:
        showDisconnectDialog();
        break;
    case CHECKING_E:
        showCheckingDialog();
        break;
    case MOVETIPS_E:
        showMoveTipsDialog();
        break;
    case TOUCHWARNING_E:
        showTouchDialog();
        break;
    }
}

void WarningDialog::showDisconnectDialog()
{
    ui->text2_Label->hide();
    QString rootPath = QApplication::applicationDirPath();
    QString text(tr("网络已断开，请检查网络!"));
    QString path("./source/dialog/connect.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/dialog/connect.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showEmergencyDialog()
{
    ui->text2_Label->hide();
    QString rootPath = QApplication::applicationDirPath();
    QString text(tr("急停按钮被按下，请复位"));
    QString path("./source/dialog/emergency.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/dialog/emergency.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showSpasmDialog()
{
    ui->text2_Label->hide();
    QString rootPath = QApplication::applicationDirPath();
    QString text(tr("请注意痉挛"));
    QString path("./source/dialog/spasm.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/dialog/spasm.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showCheckingDialog()
{
    ui->text2_Label->setVisible(true);
    ui->confirm_Btn->setVisible(false);
    ui->text2_Label->setText(tr("设备正在校准中，请稍后！"));
    ui->text_Label->hide();
    ui->image_Label->hide();
}

void WarningDialog::showMoveTipsDialog()
{
    ui->text2_Label->setVisible(true);
    ui->confirm_Btn->setVisible(true);
    ui->text2_Label->setText(tr("请将手指移开,设备即将校准！"));
    ui->text_Label->hide();
    ui->image_Label->hide();
}

void WarningDialog::showTouchDialog()
{
    ui->text2_Label->setVisible(false);
    ui->confirm_Btn->setVisible(true);
    QString text(tr("拇指与四指有接触风险，请重新设定关节活动范围"));
    QString path("./source/dialog/touchWarning.png");
    setImageAndText(path,text);
}

void WarningDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    isConfirmPressed = false;
}

void WarningDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}
void WarningDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,100));

}
void WarningDialog::setCloseTime(int second)
{
    m_closeTimer->start(second*1000);
}

void WarningDialog::slotCloseEvent()
{
    this->close();
}

void WarningDialog::setDialogOpenState(bool isShow)
{
    if(isShow)
        this->show();
    else
        this->close();
}

void WarningDialog::on_confirm_Btn_clicked()
{
    isConfirmPressed = true;
    emit signalConfirmClicked(m_type);
    this->close();
}

bool WarningDialog::getPressedState()
{
    return isConfirmPressed;
}
