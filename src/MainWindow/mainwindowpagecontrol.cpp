#include "mainwindowpagecontrol.h"
MainWindowPageControl* MainWindowPageControl::m_pageControlInterface = NULL;
MainWindowPageControl::MainWindowPageControl(QObject *parent) : QObject(parent)
{
    ST_runningFlag.evaluateFlag = false;
    ST_runningFlag.gameFlag = false;
    ST_runningFlag.moveRangeFlag = false;
    ST_runningFlag.trainModeFlag = false;
    ST_runningFlag.trainRecordFlag = false;
}

MainWindowPageControl* MainWindowPageControl::getInstance()
{
    if(m_pageControlInterface == NULL)
    {
        m_pageControlInterface = new MainWindowPageControl();
    }
    return m_pageControlInterface;
}

void MainWindowPageControl::setCurrentPage(int pageIndex)
{
    m_currentPageIndex = pageIndex;
    emit signalSwitchPage(pageIndex);
}

void MainWindowPageControl::setMainWindowBtnsLock(bool isLock)
{
    emit signalSetBtnsLock(isLock);
}

int MainWindowPageControl::getCurrentPageIndex()
{
    return  m_currentPageIndex;
}

//更新各页面状态
void MainWindowPageControl::updatePageRunningflag(E_PAGENAME page,bool flag)
{
    switch(page)
    {
    case JointMotionScopePage_E:
        ST_runningFlag.moveRangeFlag = flag;
        break;
    case TreatModePage_E:
        ST_runningFlag.trainModeFlag = flag;
        break;
    case PassiveTrainingPage_E:
    case BoostTrainingPage_E:
    case ActiveTrainingPage_E:
    case MemoryTrainingPage_E:
        ST_runningFlag.trainModeFlag = flag;
        break;
    case TrainingGamePage_E:
        ST_runningFlag.gameFlag = flag;
        break;
    case EvaluationPage_E:
        ST_runningFlag.evaluateFlag = flag;
        break;
    case StrengthEvaluationPage_E:
    case StrengthEvaluationResultPage_E:
    case MotionScopeEvaluationPage_E:
    case MotionScopeEvaluationResultPage_E:
        ST_runningFlag.evaluateFlag = flag;
        break;
    case TrainingRecordPage_E:
        ST_runningFlag.trainRecordFlag = flag;
        break;
    }
}

bool MainWindowPageControl::getPageFlag(E_PAGENAME page)
{
    bool flag;
    switch(page)
    {
    case JointMotionScopePage_E:
        flag = ST_runningFlag.moveRangeFlag;
        break;
    case TreatModePage_E:
        flag = ST_runningFlag.trainModeFlag;
        break;
    case PassiveTrainingPage_E:
    case BoostTrainingPage_E:
    case ActiveTrainingPage_E:
    case MemoryTrainingPage_E:
        flag = ST_runningFlag.trainModeFlag;
        break;
    case TrainingGamePage_E:
        flag = ST_runningFlag.gameFlag ;
        break;
    case EvaluationPage_E:
        flag = ST_runningFlag.evaluateFlag;
        break;
    case StrengthEvaluationPage_E:
    case StrengthEvaluationResultPage_E:
    case MotionScopeEvaluationPage_E:
    case MotionScopeEvaluationResultPage_E:
        flag = ST_runningFlag.evaluateFlag ;
        break;
    case TrainingRecordPage_E:
        flag = ST_runningFlag.trainRecordFlag;
        break;
    }
    return flag;
}
