#include "evaluationmanager.h"
#include "ui_evaluationmanager.h"
#include "mainwindowpagecontrol.h"
#include "dataFormate.h"
#include "mainwindowpagecontrol.h"
EvaluationManager::EvaluationManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvaluationManager)
{
    ui->setupUi(this);
}

EvaluationManager::~EvaluationManager()
{
    delete ui;
}

void EvaluationManager::on_strengthEvalution_Btn_clicked()
{
    MainWindowPageControl::getInstance()->setCurrentPage(StrengthEvaluationPage_E);
}

void EvaluationManager::on_scopeEvalution_Btn_clicked()
{
    MainWindowPageControl::getInstance()->setCurrentPage(MotionScopeEvaluationPage_E);
}

void EvaluationManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    MainWindowPageControl::getInstance()->updatePageRunningflag(EvaluationPage_E,false);
}
