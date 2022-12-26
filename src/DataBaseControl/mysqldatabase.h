#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
class MySqlDataBase : public QWidget
{
    Q_OBJECT

public:
    MySqlDataBase(QWidget *parent = nullptr);
    ~MySqlDataBase();

    //QVariantMap -- QMap<QString,QVariant> --QMap<字段名，值>
    //一下所有用到的位置都是该数据内容

    /*********打开数据库*****
     * 参数   QString name-连接名  const QString &type-数据库类型
     * 返回值 true-插入成功 false-插入失败
     * *********/
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
     * key,value是针对全部要修改的
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool updateRowTable(QString table,QString targetkey,QString targetValue,QString name,QVariantMap map);

    /*********升序排列*****
     * 参数   QString table-表名  QString target-字段名
     * key,value是针对全部要修改的
     * 返回值 true-插入成功 false-插入失败
     * *********/
    bool sortTable(QString table,QString target);

    /********获取数据库的查询结果*****
     * 说明：在使用该函数之前需要先执行查询语句,具体为
     * 参数   int page-从哪一页获取  int pageNum-一页获取多少数据
     * 返回值 QList<QVariantMap>-查询得到的结果
     * *********/
    QList<QVariantMap>getValues(int page,int pageNum);

    /****返回数据库的搜索结果个数*****/
    //在使用这个函数之前需要调用查询语句。
    int size();

    void setLastError(const QString& error);
    QString lastError();

    //获取当前数据表信息
    QMap<QString,QString> getTableInfo(QString table);

    QSqlQuery& getSqlQUery();
    bool isTableExist(QString table);

    bool prepare(const QString& query);
    bool exec(const QString &query);
    bool exec();

private:
    QSqlDatabase m_database;
    QSqlQuery m_sqlQuery;
    QString m_lastError;

};
#endif // MYSQLDATABASE_H
