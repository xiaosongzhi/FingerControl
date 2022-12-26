#include "treatmodemanager.h"
#include "ui_treatmodemanager.h"
#include "mainwindowpagecontrol.h"
#include "dataFormate.h"
#include <QDebug>
#include "ccommunicateapi.h"
TreatModeManager::TreatModeManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreatModeManager)
{
    ui->setupUi(this);
    initWidget();
}

TreatModeManager::~TreatModeManager()
{
    delete ui;
}

void TreatModeManager::initWidget()
{

}

void TreatModeManager::on_passiveTrain_Btn_clicked()
{
//    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_PASSIVE_E);
    MainWindowPageControl::getInstance()->setCurrentPage(PassiveTrainingPage_E);
}

void TreatModeManager::on_activeTrain_Btn_clicked()
{
//    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_ACTIVE_E);
    MainWindowPageControl::getInstance()->setCurrentPage(ActiveTrainingPage_E);
}

void TreatModeManager::on_boostTrain_Btn_clicked()
{
//    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_BOOST_E);
    MainWindowPageControl::getInstance()->setCurrentPage(BoostTrainingPage_E);
}

void TreatModeManager::on_memoryTrain_Btn_clicked()
{
//    CCommunicateAPI::getInstance()->setTrainMode(TRAINMODE_MEMORY_E);
    MainWindowPageControl::getInstance()->setCurrentPage(MemoryTrainingPage_E);
}

void TreatModeManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    MainWindowPageControl::getInstance()->updatePageRunningflag(TreatModePage_E,false);
}
