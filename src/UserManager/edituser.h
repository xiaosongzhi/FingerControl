#ifndef EDITUSER_H
#define EDITUSER_H

#include <QWidget>
#include "userdataformate.h"
#include "cbasedialog.h"
#include "userhandselect.h"
#include "userfingerselect.h"

namespace Ui {
class EditUser;
}

class EditUser : public CBaseDialog
{
    Q_OBJECT

public:
    explicit EditUser(QWidget *parent = nullptr);
    ~EditUser();
    //0-新建用户  1-编辑用户
    void switchPageType(QString ID,int type = 0);

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
signals:
    void signalUpdateUserList();

    /*********************************
     *说明:   完成用户选择,将页面切换至训练界面
     *@参数:  int type  0-返回用户管理界面  1-进入训练界面
     *********************************/
    void signalUserSelected(int type);
private slots:
    void on_delete_Btn_clicked();

    void on_modify_Btn_clicked();

    void on_confirm_Btn_clicked();

    void on_save_Btn_clicked();

    void on_male_RadioButton_clicked();

    void on_female_RadioButton_clicked();

    //连接右上角关闭按钮
    void slotCloseBtn();
    //从选择手的页面返回到本页面
    void slotBackToEditUserPage();

private:
    void addUserPage();
    void editUserPage(QString certificateNum);
    //设置lineEdit的输入限制
    void setLineEditFormat();
    //设置用户信息
    void setUserData(ST_PatientMsg);
    //获取用户数据
    ST_PatientMsg getUserData();
    //清理用户数据
    void clearUserData();
    //初始化控件
    void initWidgetUI();
    //写配置
    void writeSetting();
    //读配置
    void readSetting();
private:
    Ui::EditUser *ui;

    //已有用户则使用手选择，在手选择中可以添加手指选择
    UserHandSelect *m_handSelect;

    //新用户的时候使用手指添加
    UserFingerSelect *m_fingerSelect;

    uint32_t m_currentID; //系统给用户分配的ID

    ST_PatientMsg st_CurrentUserData;
};

#endif // EDITUSER_H
