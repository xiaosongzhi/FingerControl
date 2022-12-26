#include "cbasedialog.h"
#include "ui_cbasedialog.h"
#include <QStyleOption>
#include <QPainter>
CBaseDialog::CBaseDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CBaseDialog)
{
    ui->setupUi(this);
    resize(1500,800);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

}

CBaseDialog::~CBaseDialog()
{
    delete ui;
}

void CBaseDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    float xCoefficient = rect().width()/1500.f;
    float yCoefficient = rect().height()/800.f;

    ui->groupBox->setGeometry(0,0,rect().width(),80*yCoefficient);


    int titleX = (ui->groupBox->width()-ui->title_Label->width())/2;
    int titleY = (ui->groupBox->height()-ui->title_Label->height())/2;
    //设置标题位置
    ui->title_Label->setGeometry(titleX,titleY,300*xCoefficient,80*yCoefficient);

    //设置关闭按钮
    int closeBtnX = 1426 * xCoefficient;
    int closeBtnY = 18 * yCoefficient;

    ui->close_Btn->setGeometry(closeBtnX,closeBtnY,44*xCoefficient,44*xCoefficient);
}

void CBaseDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
//    rect.setWidth(rect.width() - 1);
//    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 8, 8);
    QWidget::paintEvent(event);
}

void CBaseDialog::on_close_Btn_clicked()
{
//    this->close();
    emit signalCloseEvent();
}

void CBaseDialog::setTitile(QString title)
{
    ui->title_Label->setText(title);
}
