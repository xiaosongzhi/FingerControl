#ifndef STRENGTHEVALUATIONRESULT_H
#define STRENGTHEVALUATIONRESULT_H

#include <QWidget>
#include "currentuserdata.h"
namespace Ui {
class StrengthEvaluationResult;
}

class StrengthEvaluationResult : public QWidget
{
    Q_OBJECT

public:
    explicit StrengthEvaluationResult(QWidget *parent = nullptr);
    ~StrengthEvaluationResult();
public slots:
    //关闭当前界面的训练
    void slotTurnOffRunning(int pageIndex);
protected:
    virtual void showEvent(QShowEvent *event);
private slots:
    void on_finish_Btn_clicked();
private:
    void displayResultData();
    bool saveImageResult();
private:
    Ui::StrengthEvaluationResult *ui;
    ST_FringerStrengthResult m_st_fingerStrengthResult;
};

#endif // STRENGTHEVALUATIONRESULT_H
