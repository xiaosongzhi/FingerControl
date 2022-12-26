#ifndef CPASSWORDDIALOG_H
#define CPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class CPasswordDialog;
}

class CPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPasswordDialog(QWidget *parent = nullptr);
    ~CPasswordDialog();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
    void signalResult(int);
private slots:
    void on_confirm_Btn_clicked();

    void on_cancel_Btn_clicked();

    void on_close_Btn_clicked();

private:
    void setLineEditInput();

private:
    Ui::CPasswordDialog *ui;
};

#endif // CPASSWORDDIALOG_H
