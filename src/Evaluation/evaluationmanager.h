#ifndef EVALUATIONMANAGER_H
#define EVALUATIONMANAGER_H

#include <QWidget>

namespace Ui {
class EvaluationManager;
}

class EvaluationManager : public QWidget
{
    Q_OBJECT

public:
    explicit EvaluationManager(QWidget *parent = nullptr);
    ~EvaluationManager();
protected:
    void showEvent(QShowEvent *event);
private slots:
    void on_strengthEvalution_Btn_clicked();

    void on_scopeEvalution_Btn_clicked();
private:
    Ui::EvaluationManager *ui;


};

#endif // EVALUATIONMANAGER_H
