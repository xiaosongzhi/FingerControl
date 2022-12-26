#include "movescopeprogressbar.h"
#include "ui_movescopeprogressbar.h"

MoveScopeProgressBar::MoveScopeProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveScopeProgressBar)
{
    ui->setupUi(this);
}

MoveScopeProgressBar::~MoveScopeProgressBar()
{
    delete ui;
}
