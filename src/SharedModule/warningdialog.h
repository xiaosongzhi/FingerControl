#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QWidget>
#include "dataFormate.h"
#include "cbasedialog.h"

class QTimer;

namespace Ui {
class WarningDialog;
}

class WarningDialog : public QWidget
{
    Q_OBJECT

public:
    explicit WarningDialog(QWidget *parent = nullptr);
    ~WarningDialog();

    void setWarningType(E_WarningType);
    //设置倒计时关闭按钮
    void setCloseTime(int second);

    void setDialogOpenState(bool);

    bool getPressedState();
private:
    void setImageAndText(QString path,QString text);

    void showEmergencyDialog();

    void showSpasmDialog();

    void showDisconnectDialog();

    void showCheckingDialog();

    void showMoveTipsDialog();

    void showTouchDialog();

    void loadStyleSheetFile();
private slots:
    void slotCloseEvent();
    void on_confirm_Btn_clicked();
signals:
    void signalConfirmClicked(E_WarningType type);
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::WarningDialog *ui;
    QTimer *m_closeTimer;
    E_WarningType m_type;
    bool isConfirmPressed;
};

#endif // WARNINGDIALOG_H
