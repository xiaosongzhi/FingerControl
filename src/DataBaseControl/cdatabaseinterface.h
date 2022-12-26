#ifndef CDATABASEINTERFACE_H
#define CDATABASEINTERFACE_H

#include <QObject>
#include <QVariant>
#include <QString>
class MySqlDataBase;
class CDatabaseInterface : public QObject
{
    Q_OBJECT
public:
    static CDatabaseInterface* getInstance();

    bool openDB(QString name,const QString &type = "QMYSQL");

    //关闭数据库
    void closeDB();

    /*********创建数据表*****
     * 参数   QString table-表名 QMap<QString,QString>-(arg1字段名)(arg2数据类型)
     * 返回值 true-插入成功 false-插入失败
     * 备注：最好不要使用该函数建表，因为涉及到的语句比较特殊，容易出错
     * *********/
    bool  createTable(QString table,QMap<QString,QString>);

    /*********数据库插入操作*****
     * 参数   QString table-表名  QVariantMap map-实际带插入数据
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool insertRowTable(QString table,QVariantMap map);

    /*********删除数据库中的某一行*****
     * 参数   QString table-表名  QString columName-列字段  QString value-需要删除的具体数据
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool deleteRowTable(QString table,QString columName,QString value);

    /*********更新数据库中的某一行*****
     * 参数   QString table-表名  QString name-字段名  QVariantMap map-具体数据
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool updateRowTable(QString table,QString name,QVariantMap map);

    /*********更新数据库中的某一行*****
     * 参数   QString table-表名  QString targetkey-字段名 QString targetValue-具体值 QVariantMap map-具体数据
     * targetkey,targetValue为查询条件
     * name查询条件二
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool updateRowTable(QString table,QString targetkey,QString targetValue,QString name,QVariantMap map);

    /*********升序排列*****
     * 参数   QString table-表名  QString target-字段名
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool sortTable(QString table,QString target);

    /********获取数据库的查询结果*****
     * 说明：在使用该函数之前需要先执行查询语句,具体为
     * 参数   int page-从哪一页获取  int pageNum-一页获取多少数据
     * 返回值 QList<QVariantMap>-查询得到的结果
     * *********/
    QList<QVariantMap>getValues(int page,int pageNum);

    bool prepare(const QString& query);
    bool exec(const QString &query);
    bool exec();
    //获取数据库操作最后出现的错误
    QString getLastError();

    int getValuesSize();
private:
    explicit CDatabaseInterface(QObject *parent = nullptr);

    static CDatabaseInterface* m_DatabaseInterface;

    MySqlDataBase *m_sqlDataBase;
};

#endif // CDATABASEINTERFACE_H
