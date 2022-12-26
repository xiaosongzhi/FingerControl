#ifndef CONTROLMASKDIALOG_H
#define CONTROLMASKDIALOG_H

#include <QObject>
#include "cmaskdialog.h"
class ControlMaskDialog : public QObject
{
    Q_OBJECT
private:
    explicit ControlMaskDialog(QObject *parent = nullptr);

    CmaskDialog m_maskDialog;

    static ControlMaskDialog* m_controlDialog;

    bool isHide;
public:
    static ControlMaskDialog* getInstace();

    void setMaskDialogHide(bool);

    bool getMaskDialogHideState();

signals:

};

#endif // CONTROLMASKDIALOG_H
