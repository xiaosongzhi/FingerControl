#include "controlmaskdialog.h"

ControlMaskDialog* ControlMaskDialog::m_controlDialog = NULL;

ControlMaskDialog::ControlMaskDialog(QObject *parent) : QObject(parent)
{

}

void ControlMaskDialog::setMaskDialogHide(bool hide)
{
    isHide = hide;
    if(hide)
        m_maskDialog.setHide();
    else
        m_maskDialog.setDisplay();
}

ControlMaskDialog* ControlMaskDialog::getInstace()
{
    if(NULL == m_controlDialog)
    {
        m_controlDialog = new ControlMaskDialog();
    }
    return m_controlDialog;
}

bool ControlMaskDialog::getMaskDialogHideState()
{
    return isHide;
}
