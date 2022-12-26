#ifndef CMASKDIALOG_H
#define CMASKDIALOG_H

#include <QWidget>

class CmaskDialog : public QWidget
{
    Q_OBJECT
public:
    explicit CmaskDialog(QWidget *parent = nullptr);

    void setDisplay();
    void setHide();
    void setClose();

signals:

};

#endif // CMASKDIALOG_H
