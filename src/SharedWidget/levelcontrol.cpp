#include "levelcontrol.h"
#include "ui_levelcontrol.h"

LevelControl::LevelControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LevelControl)
{
    ui->setupUi(this);
}

LevelControl::~LevelControl()
{
    delete ui;
}
