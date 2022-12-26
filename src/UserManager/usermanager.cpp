#include "usermanager.h"
#include "ui_usermanager.h"
#include <QDebug>
#include <QMetaType>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "cdatabaseinterface.h"
#include "userdataformate.h"
#include <QListWidget>
#include "controlmaskdialog.h"
#include <QFile>
#include "mainwindowpagecontrol.h"
UserManager::UserManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManager),
    m_sequenceGroup(NULL),
    m_editUser(NULL),
    m_currentPage(1),
    m_totalPage(1),
    m_totalUserNum(0)
{
    ui->setupUi(this);
    //设置窗体透明
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);    //在windows环境下需要打开
    setAttribute(Qt::WA_TranslucentBackground);

    m_sequenceGroup = new QParallelAnimationGroup(this);
    m_editUser = new EditUser();
    connect(m_editUser,SIGNAL(signalUpdateUserList()),this,SLOT(slotUpdateUserList()));
    connect(m_editUser,SIGNAL(signalUserSelected(int)),this,SLOT(slotUserSelected(int)));
    updateRecipeList();
    //隐藏测试按钮

    loadStyleSheetFile();

    connect(ui->search_LineEdit,SIGNAL(returnPressed()),this,SLOT(slotEnterPressed()));
}

UserManager::~UserManager()
{
    if(snapLabel)
    {
        delete snapLabel;
        snapLabel = NULL;
    }
    if(m_sequenceGroup)
    {
        m_sequenceGroup->clear();
        delete  m_sequenceGroup;
        m_sequenceGroup = NULL;
    }
    if(m_editUser)
    {
        delete  m_editUser;
        m_editUser = NULL;
    }

    delete ui;
}



//刷新用户管理界面
void UserManager::updateRecipeList()
{
    QString query("select * from patientmessagetable");
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    m_totalUserNum = count;
    m_userValueList.clear();
    m_userValueList = CDatabaseInterface::getInstance()->getValues(0,count);
    //总共要放的页数
    int pages = count/PERCOUNT + 1;
    m_totalPage = pages;
    ui->page_Label->setText(QString("1/%1").arg(pages));

    if(1 == pages)
    {
        ui->left_Btn->setVisible(false);
        ui->right_Btn->setVisible(false);
    }
    else
    {
        ui->left_Btn->setVisible(true);
        ui->right_Btn->setVisible(true);
    }
    showUserData(1);
}

void UserManager::slotSelectItemDoubleClicked(QListWidgetItem *item)
{
    QString itemText = item->text();
    if(itemText.contains("\r"))
    {
        int index = itemText.indexOf("\r");
        itemText.remove(0,index+2);
    }
    if(itemText == tr("添加用户"))
    {
        //隐藏用户管理界面
        this->hide();
        //添加新用户
        m_editUser->switchPageType(0);
        //显示用户编辑界面
        m_editUser->show();
    }
    else    //编辑用户
    {
        this->hide();
        //查看编辑其他用户
        m_editUser->switchPageType(itemText,1);

        m_editUser->show();
    }
}

void UserManager::on_search_Btn_clicked()
{
    //首先清理界面
    ui->listWidget->clear();
    //检索的情况下不显示添加按钮，只显示符合检索条件的内容
    QString msg = ui->search_LineEdit->text();
    QString query(QString("select * from patientmessagetable where name = '%1' or id = '%2'").arg(msg).arg(msg));

    QList<QVariantMap> valueMapList;
    if(CDatabaseInterface::getInstance()->exec(query))
    {
        valueMapList = CDatabaseInterface::getInstance()->getValues(0,8);
    }
    if(0 == valueMapList.count())
    {
        qDebug()<<"未找到合适数据";
    }

    QListWidget *listWidget = ui->listWidget;
    connect(listWidget,&QListWidget::itemDoubleClicked,this,&UserManager::slotSelectItemDoubleClicked);
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setMovement(QListView::Static);
    listWidget->setIconSize(QSize(90,118));
    listWidget->setSpacing(20);

    QFont font;
    font.setFamily("黑体");
    font.setPointSize(20);
    //要添加的个数
    int addNum = qMin(valueMapList.count(),PERCOUNT);
    for(int i = 0;i < addNum;i++)
    {
        QVariantMap vMap = valueMapList.at(i);
        //患者性别
        int sex = vMap.value("sex").toInt();
        QString pixPath;
        if(0 == sex)//male
            pixPath = ":/new/prefix1/source/image/male1.png";
        else if(1 == sex)//female
            pixPath = ":/new/prefix1/source/image/female1.png";
        //患者ID号
        QString IDNum = vMap.value("ID").toString();
        //患者姓名
        QString name = vMap.value("name").toString();
        if(name.isEmpty())
            continue;
        QString text = name+"\r\n"+IDNum;
        QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
        addItem->setFont(font);
        listWidget->addItem(addItem);
    }
}

void UserManager::on_left_Btn_clicked()
{
    if(m_currentPage < 2)
        return;
    m_currentPage--;
    showUserData(m_currentPage);
}

void UserManager::on_right_Btn_clicked()
{
    if(m_currentPage < m_totalPage)
    {
        m_currentPage++;
        showUserData(m_currentPage);
    }
}

void UserManager::slotUpdateUserList()
{
//    updateRecipeList();
}

void UserManager::slotUserSelected(int type)
{
    //返回用户管理界面
    if(0 == type)
    {
        this->show();
    }
    //进入训练界面
    else if(1 == type)
    {
        close();
        ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    }
}

void UserManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    MainWindowPageControl::getInstance()->setCurrentPage(EmptyPage_E);
    ControlMaskDialog::getInstace()->setMaskDialogHide(false);
    updateRecipeList();
}

void UserManager::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

void UserManager::showUserData(uint16_t pageIndex)
{
    ui->listWidget->clear();
    QListWidget *listWidget = ui->listWidget;
    connect(listWidget,&QListWidget::itemDoubleClicked,this,&UserManager::slotSelectItemDoubleClicked);
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setMovement(QListView::Static);
    listWidget->setIconSize(QSize(90,118));
    listWidget->setSpacing(20);
    QFont font;
    font.setFamily("黑体");
    font.setPointSize(20);
    //第一页
    if(1 == pageIndex)
    {
        //添加 “添加”Item
        QListWidgetItem *addItem = new QListWidgetItem(QIcon(":/new/prefix1/source/image/add.png"),tr("添加用户"));

        addItem->setFont(font);
        listWidget->addItem(addItem);
        //添加其他患者,第一页最多添加七个
        int addNum = qMin(m_totalUserNum,PERCOUNT-1);
        if(m_userValueList.size() >= addNum)
            for(int i = 0;i < addNum;i++)
            {
                QVariantMap vMap = m_userValueList.at(i);
                //患者性别
                int sex = vMap.value("sex").toInt();
                QString pixPath;
                if(0 == sex)//male
                    pixPath = ":/new/prefix1/source/image/male1.png";
                else if(1 == sex)//female
                    pixPath = ":/new/prefix1/source/image/female1.png";
                //患者ID号
                QString IDNum = vMap.value("ID").toString();
                //患者姓名
                QString name = vMap.value("name").toString();
                QString text = name+"\r\n"+IDNum;
                QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
                addItem->setFont(font);
                listWidget->addItem(addItem);
            }
    }
    //其它页
    else
    {
        //根据页数计算要添加的内容
        int currentUserIndex = (pageIndex-2)*PERCOUNT + (PERCOUNT-1);
        int tobeAdd = m_totalUserNum - (pageIndex-2)*PERCOUNT - (PERCOUNT-1);

        tobeAdd = qMin(tobeAdd,PERCOUNT);

        for(int i = 0;i < tobeAdd;i++)
        {
            if(m_userValueList.size() >i+currentUserIndex)
            {
                QVariantMap vMap = m_userValueList.at(i+currentUserIndex);
                //患者性别
                int sex = vMap.value("sex").toInt();
                QString pixPath;
                if(0 == sex)//male
                    pixPath = ":/new/prefix1/source/image/male1.png";
                else if(1 == sex)//female
                    pixPath = ":/new/prefix1/source/image/female1.png";
                //患者身份证号
                QString ID = vMap.value("ID").toString();
                //患者姓名
                QString name = vMap.value("name").toString();
                QString text = name+"\r\n"+ID;
                QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
                addItem->setFont(font);
                listWidget->addItem(addItem);
            }
        }
    }
}
void UserManager::loadStyleSheetFile()
{
    //加载样式文件
    QFile qssFile("./QSS/usermanager.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        qDebug()<<"usermanager.qss文件打开失败!";
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}
void UserManager::on_search_LineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        updateRecipeList();
    }
}

void UserManager::slotEnterPressed()
{
    qDebug()<<"pressed";
}
