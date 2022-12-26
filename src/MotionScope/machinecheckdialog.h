#ifndef MACHINECHECKDIALOG_H
#define MACHINECHECKDIALOG_H

#include <QDialog>
#include <QTimer>
#define  MACHINECHECKTIME 100*300
namespace Ui {
class MachineCheckDialog;
}

class MachineCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MachineCheckDialog(QWidget *parent = nullptr);
    ~MachineCheckDialog();

    void updateProcess(int value);

private slots:
    void slotUpdateProcess();

//    void receiveCommData(QByteArray);
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
private:
    Ui::MachineCheckDialog *ui;
    QTimer *m_timer;
};

#endif // MACHINECHECKDIALOG_H
