#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "usermanager.h"
#include "warningdialog.h"
#include "displaywindow.h"
class QButtonGroup;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void switchUser();
private slots:
    void slotBtnGroupClicked(int btnId);

    void slotCheckbuttonClicked(int id);

    void on_jointScope_Btn_clicked();

    void on_treatMode_Btn_clicked();

    void on_trainingGame_Btn_clicked();

    void on_evaluation_Btn_clicked();

    void on_trainingRecord_Btn_clicked();

    void on_setting_Btn_clicked();

    void on_shutDown_Btn_clicked();

    void on_switchUser_Btn_clicked();

    void on_startCheck_Btn_clicked();

    void on_stopCheck_Btn_clicked();

public slots:
    //切换不同界面
    void slotSwitchCurrentPage(int pageIndex);
    //左侧按钮的锁定与解锁
    void slotBtnsLock(bool);
    //用户切换
    void slotCurrentUserChanged();
    //心跳槽函数
    void slotHeartBeatSlot();
    //接收下位机数据
    void slotReceiveData(QByteArray);

    void slotWatchSend(QByteArray);
private:
    void setBtnsLcok(bool isLock);
    void initWidgetUI();
    void initButtonGroup();

    void setCurrentUser();
private:
    Ui::MainWindow *ui;
    QButtonGroup *m_buttonGroup;
    UserManager *m_userManager;
    QTimer *m_heartBeatTimer;
    QLabel *m_emergencyLabel;
    QLabel *m_connectStateLabel;
    bool m_isInEmergency;
    WarningDialog *m_disconnectDialog;
    int m_heartbeatLostTimes; //心跳失联检测
    bool m_connectState;      //网络连接状态
    bool m_lastConnectState;  //之前的状态
    DisplayWindow *m_displayWindow;

    QButtonGroup *m_checkButtonGroup;
};
#endif // MAINWINDOW_H
