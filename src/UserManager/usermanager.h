#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include "edituser.h"

class QLabel;
class QParallelAnimationGroup;
class QListWidgetItem;
namespace Ui {
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);
    ~UserManager();
signals:
    void signalSwitchStackPage(int index);
private slots:
    void on_search_Btn_clicked();

    void on_left_Btn_clicked();

    void on_right_Btn_clicked();

    void slotSelectItemDoubleClicked(QListWidgetItem *item);

    void slotUpdateUserList();

    /****完成用户选择,将页面切换至训练界面
     * @参数int type  0-返回用户管理界面  1-进入训练界面
     ******/
    void slotUserSelected(int type);

    void on_search_LineEdit_textChanged(const QString &arg1);

    void slotEnterPressed();
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
private:
    void dynamicAddButton();
    //初始化第一页
    void initFirstPage();

    //刷新用户管理界面
    void updateRecipeList();

    //显示用户数据 页数从1开始
    void showUserData(uint16_t pageIndex);

    //加载样式表
    void loadStyleSheetFile();

private:
    Ui::UserManager *ui;
    QLabel *snapLabel;
    QParallelAnimationGroup *m_sequenceGroup;
    EditUser *m_editUser;
    int m_currentPage;
    int m_totalPage;
    int m_totalUserNum;
    QList<QVariantMap> m_userValueList;
};

#endif // USERMANAGER_H
