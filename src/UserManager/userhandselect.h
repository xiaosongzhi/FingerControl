#ifndef USERHANDSELECT_H
#define USERHANDSELECT_H

#include <QWidget>
#include <QPushButton>
#include "cbasedialog.h"
#include "userfingerselect.h"
#include "currentuserdata.h"

namespace Ui {
class UserHandSelect;
}

class UserHandSelect : public CBaseDialog
{
    Q_OBJECT

public:
    explicit UserHandSelect(QWidget *parent = nullptr);
    ~UserHandSelect();

    /************************************
    *说明:设置两个按钮的style
    *参数：
    *@int type： 0-添加+右手  1-添加+左手  2-左手+右手
    *返回值：无
    ********************************/
    void setHandBtns(int type);

    //获取待测定的手 0-左  1-右
    int getTrainHand();
signals:
    void signalBackToEditUserPage();
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_hand1_Btn_clicked();

    void on_hand2_Btn_clicked();

    void on_confirm_Btn_clicked();

    //连接右上角关闭按钮
    void slotCloseBtn();
private:
    void initWidget();

    void setBtnStyle(QPushButton*,QString);
    //根据被选中的手获取手指的数据
    bool getHandData(int selectHand,ST_HandData& st_HandData);
private:
    Ui::UserHandSelect *ui;
    QPair<QString,QString> addStyle;
    QPair<QString,QString> leftHandStyle;
    QPair<QString,QString> rightHandStyle;
    UserFingerSelect *m_userFingerSelect;
    int m_handType; //0-左手  1-右手  2-双手
    int selectedHand;//被选中的手
};

#endif // USERHANDSELECT_H
