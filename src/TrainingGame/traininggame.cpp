#include "traininggame.h"
#include "ui_traininggame.h"
#include "dataFormate.h"
#include "mainwindowpagecontrol.h"

TrainingGame::TrainingGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingGame)
{
    ui->setupUi(this);
}

TrainingGame::~TrainingGame()
{
    delete ui;
}

void TrainingGame::on_memoryTrain_Btn_clicked()
{
    MainWindowPageControl::getInstance()->setCurrentPage(MemoryTrainingPage_E);
}
