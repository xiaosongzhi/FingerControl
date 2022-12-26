#include "virtualhandle.h"
#include "ui_virtualhandle.h"

VirtualHandle::VirtualHandle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VirtualHandle)
{
    ui->setupUi(this);
}

VirtualHandle::~VirtualHandle()
{
    delete ui;
}
