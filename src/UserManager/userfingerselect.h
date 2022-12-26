#ifndef USERFINGERSELECT_H
#define USERFINGERSELECT_H

#include <QWidget>
#include "cbasedialog.h"
#include <QButtonGroup>

namespace Ui {
class UserFingerSelect;
}

class UserFingerSelect : public CBaseDialog
{
    Q_OBJECT

public:
    explicit UserFingerSelect(QWidget *parent = nullptr);
    ~UserFingerSelect();
    //设置左右手
    void setLeftRightHand(int handType);
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:

    void on_confirm_Btn_clicked();

    void on_left_RadioButton_clicked(bool checked);

    void on_right_RadioButton_clicked(bool checked);

    void slotButtonToogled(int id,bool checked);
private:
    Ui::UserFingerSelect *ui;

    QButtonGroup *m_radioButtonGroup;

    int m_handType; //0-左手 1-右手
};

#endif // USERFINGERSELECT_H
