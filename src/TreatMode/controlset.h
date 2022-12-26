#ifndef CONTROLSET_H
#define CONTROLSET_H

#include <QWidget>
#include "cbasedialog.h"
class QButtonGroup;
namespace Ui {
class ControlSet;
}
typedef struct
{
    int8_t controlType;     //1-力量趋势控制 2-位移控制
    int8_t finger[5] = {0}; //0-未选中 1-选中
    int8_t bucklingLevel;   //取值1,2,3代表三个等级，在controlType=1时有效
    int8_t stretchLevel;    //取值1,2,3代表三个等级，在controlType=1时有效
}ST_ControlParam;

class ControlSet : public CBaseDialog
{
    Q_OBJECT

public:
    explicit ControlSet(QWidget *parent = nullptr);
    ~ControlSet();

    ST_ControlParam getControlParam();
private slots:
    void slotBucklingBtnGroup(int);
    void slotStretchBtnGroup(int);
    void on_confirm_Btn_clicked();

    void on_tendency_Btn_clicked();

    void on_displacement_Btn_clicked();

    void on_stretchLevel2_Btn_clicked();

    void slotCloseEvent();
protected:
    virtual void showEvent(QShowEvent* event);
    virtual void closeEvent(QCloseEvent* event);

private:
    void initWidget();
    void flushFingerState();
private:
    Ui::ControlSet *ui;
    QStringList levelNormalList;
    QStringList levelSelectedList;

    QButtonGroup *m_bucklingLevelGroup;
    QButtonGroup *m_stretchLevelGroup;

    QPair<QString,QString> displacementStyle;
    QPair<QString,QString> tendencyStyle;

    ST_ControlParam st_ControlParam;
};

#endif // CONTROLSET_H
