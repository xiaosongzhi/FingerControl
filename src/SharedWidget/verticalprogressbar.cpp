#include "verticalprogressbar.h"
#include "ui_verticalprogressbar.h"

VerticalProgressBar::VerticalProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerticalProgressBar)
{
    ui->setupUi(this);
}

VerticalProgressBar::~VerticalProgressBar()
{
    delete ui;
}
