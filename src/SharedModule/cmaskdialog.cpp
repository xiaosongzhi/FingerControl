#include "cmaskdialog.h"
#include <QPalette>

CmaskDialog::CmaskDialog(QWidget *parent) : QWidget(parent)
{
    resize(1920,1080);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(0x00,0x00,0x00));
    this->setPalette(palette);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.5);
}

void CmaskDialog::setDisplay()
{
    this->show();
}
void CmaskDialog::setHide()
{
    this->hide();
}
void CmaskDialog::setClose()
{
    this->close();
}
