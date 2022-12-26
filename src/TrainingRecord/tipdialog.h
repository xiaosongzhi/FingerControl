#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QWidget>

namespace Ui {
class TipDialog;
}

class TipDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TipDialog(QWidget *parent = nullptr);
    ~TipDialog();

private:
    Ui::TipDialog *ui;
};

#endif // TIPDIALOG_H
