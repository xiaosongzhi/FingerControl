#ifndef CBASEDIALOG_H
#define CBASEDIALOG_H

#include <QWidget>

namespace Ui {
class CBaseDialog;
}

class CBaseDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CBaseDialog(QWidget *parent = nullptr);
    ~CBaseDialog();

    void setTitile(QString title);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void signalCloseEvent();

private slots:
    void on_close_Btn_clicked();

private:
    Ui::CBaseDialog *ui;
};

#endif // CBASEDIALOG_H
