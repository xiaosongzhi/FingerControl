#ifndef MOTIONSCOPEEVALUATIONRESULT_H
#define MOTIONSCOPEEVALUATIONRESULT_H

#include <QWidget>
#include "currentuserdata.h"
namespace Ui {
class MotionScopeEvaluationResult;
}

class MotionScopeEvaluationResult : public QWidget
{
    Q_OBJECT

public:
    explicit MotionScopeEvaluationResult(QWidget *parent = nullptr);
    ~MotionScopeEvaluationResult();
public slots:
    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);
protected:
    virtual void showEvent(QShowEvent *event);
private slots:
    void on_finish_Btn_clicked();
private:
    void displayResult();
    //保存图片
    bool saveImageResult();
private:
    Ui::MotionScopeEvaluationResult *ui;
    ST_JointRangeResult m_st_jointRangeResult;
};

#endif // MOTIONSCOPEEVALUATIONRESULT_H
