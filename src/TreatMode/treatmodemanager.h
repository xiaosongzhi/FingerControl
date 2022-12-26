#ifndef TREATMODEMANAGER_H
#define TREATMODEMANAGER_H

#include <QWidget>
#include <QShowEvent>
namespace Ui {
class TreatModeManager;
}

class TreatModeManager : public QWidget
{
    Q_OBJECT

public:
    explicit TreatModeManager(QWidget *parent = nullptr);
    ~TreatModeManager();
protected:
    virtual void showEvent(QShowEvent *event);

private:
    void initWidget();
private slots:
    void on_passiveTrain_Btn_clicked();

    void on_activeTrain_Btn_clicked();

    void on_boostTrain_Btn_clicked();

    void on_memoryTrain_Btn_clicked();

private:
    Ui::TreatModeManager *ui;

    QPair<QString,QString> passiveTrainStyle;
    QPair<QString,QString> activeTrainStyle;
    QPair<QString,QString> boostTrainStyle;
    QPair<QString,QString> memoryTrainStyle;
};

#endif // TREATMODEMANAGER_H
