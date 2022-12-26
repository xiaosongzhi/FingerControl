#include "mysqldatabase.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
MySqlDataBase::MySqlDataBase(QWidget *parent)
    : QWidget(parent)

{

}

MySqlDataBase::~MySqlDataBase()
{

}

/*********打开数据库***************/
bool MySqlDataBase::openDB(QString name,const QString &type)
{
    //首先判断是否该数据库
    if(QSqlDatabase::contains(name))
    {
        m_database = QSqlDatabase::database(name);
    }
    else
    {
        /***根据type类型添加数据库驱动****/
        m_database = QSqlDatabase::addDatabase(type,name);
        /****设置数据库连接名****/
        m_database.setDatabaseName(name);
    }

    m_database.setUserName("root");
    m_database.setPassword("root");

    if(!m_database.open())
    {
        setLastError(m_database.lastError().text());
        return false;
    }
    m_sqlQuery = QSqlQuery(m_database);

    qDebug()<<QSqlDatabase::connectionNames();

    return true;
}

void MySqlDataBase::closeDB()
{
    if(m_database.isOpen())
        m_database.close();
}

/*********创建数据库表格**************/
bool  MySqlDataBase::createTable(QString table,QMap<QString,QString> map)
{
    if(!isTableExist(table))
    {
        QString tableList = QString("create table if not exists %1 (").arg(table);
        QMapIterator<QString,QString> iter(map);
        while(iter.hasNext())
        {
            iter.next();
            tableList.append(QString("%1 %2").arg(iter.key()).arg(iter.value()));
            if(iter.hasNext())
                tableList.append(",");
        }
        tableList.append(")");
        //执行sql语句
        return this->exec(tableList);
    }
    else
    {
        setLastError(QString("Exist table<%1>").arg(table));
        return false;
    }
}

/*********数据库插入操作**************/
bool MySqlDataBase::insertRowTable(QString table,QVariantMap map)
{
    QMap<QString,QString> tableContentMap;
    if(!isTableExist(table))
    {
        setLastError(QString("Not find %1 table").arg(table));
        return false;
    }
    else
    {
        tableContentMap = getTableInfo(table);
        if(tableContentMap.isEmpty())
            return false;
    }

    QString insertTableContent = QString("insert into %1(").arg(table);
    QString values = QString(" values(");

    QMapIterator<QString,QString> iter(tableContentMap);

    while(iter.hasNext())
    {
        iter.next();
        insertTableContent.append(QString("%1").arg(iter.key()));
        values.append("?");
        if(iter.hasNext())
        {
            insertTableContent.append(", ");
            values.append(",");
        }
    }
    insertTableContent.append(")");
    values.append(")");

    insertTableContent.append(values);

    if(!this->prepare(insertTableContent))
        return false;
    QMapIterator<QString,QString> iter_(tableContentMap);
    while(iter_.hasNext())
    {
        iter_.next();
        m_sqlQuery.addBindValue(map[iter_.key()]);
    }
    return this->exec();
}

/*********删除数据库中的某一行*********/
bool MySqlDataBase::deleteRowTable(QString table,QString columName,QString value)
{
    QString delereContent = QString("delete from %1 where %2 = '%3'").arg(table).arg(columName).arg(value);

    return this->exec(delereContent);
}

/*********更新/更改某一个字段的n行数据*********/
bool MySqlDataBase::updateRowTable(QString table,QString name,QVariantMap map)
{
    QString content = QString("update %1 set ").arg(table);
    QMapIterator<QString,QVariant> iter(map);

    while(iter.hasNext())
    {
        iter.next();
        if(iter.hasNext())
            content += QString("%1 = '%2',").arg(iter.key()).arg(iter.value().toString());
        else
            content += QString("%1 = '%2' ").arg(iter.key()).arg(iter.value().toString());
    }
    content += QString("where %1 = '%2'").arg(name).arg(map[name].toString());
    return this->exec(content);
}

/*********更新某个字段为特定值的其他字段的值********/
bool MySqlDataBase::updateRowTable(QString table,QString targetkey,QString targetValue,QString name,QVariantMap map)
{
    QString content = QString("update %1 set ").arg(table);
    QMapIterator<QString,QVariant> iter(map);
    while(iter.hasNext())
    {
        iter.next();
        if(iter.key() == "UUID")
            continue;
        if(iter.hasNext())
            content += QString("%1 = '%2',").arg(iter.key()).arg(iter.value().toString());
        else
            content += QString("%1 = '%2' ").arg(iter.key()).arg(iter.value().toString());
    }

    content += QString("where %1 = '%2' and %3 = '%4'").arg(targetkey).arg(targetValue).arg(name).arg(map[name].toString());

    return this->exec(content);
}

/*********升序排列*************/
bool MySqlDataBase::sortTable(QString table,QString target)
{
    /***select * from table by target*/
    QString sortContent = QString("select * from %1 order by '%2'").arg(table).arg(target);
    return this->exec(sortContent);
}

/********获取数据库的查询结果*******/
QList<QVariantMap> MySqlDataBase::getValues(int page,int pageNum)
{
    QList<QVariantMap>list;
    bool ok = m_sqlQuery.isSelect();

    if(!ok)
        return QList<QVariantMap>();
    if( (!m_sqlQuery.seek(page)))
    {
        setLastError("gevalues error![The number of pages is out of the limit]");
    }
    do
    {
        QVariantMap map;
        for(int i = 0;i < m_sqlQuery.record().count();i++)
        {
            map.insert(m_sqlQuery.record().field(i).name(),
                       m_sqlQuery.record().field(i).value());

        }
        list.append(map);
    }while(m_sqlQuery.next() && --pageNum);

    return list;
}

/****获取个数*****/
int MySqlDataBase::size()
{
    int size = -1;

    while(m_sqlQuery.isSelect() && m_sqlQuery.next())
    {
        /***驱动支持返回记录***/
        if(m_sqlQuery.driver()->hasFeature(QSqlDriver::QuerySize))
        {
            size = m_sqlQuery.size();
            break;
        }
        else
        {
            m_sqlQuery.last();
            size = m_sqlQuery.at() + 1 ;
        }
    }
    //返回第一个结果
    m_sqlQuery.first();
    return size;
}

QString MySqlDataBase::lastError()
{
    return m_lastError;
}
void MySqlDataBase::setLastError(const QString& error)
{
    m_lastError = error;
}
QSqlQuery& MySqlDataBase::getSqlQUery()
{
    return m_sqlQuery;
}
bool MySqlDataBase::isTableExist(QString table)
{
    QStringList tables = m_database.tables();
    if(tables.contains(table))
        return true;
    else
        return false;
}

bool MySqlDataBase::prepare(const QString& query)
{
    if(!m_sqlQuery.prepare(QString(query)))
    {
        setLastError(m_sqlQuery.lastError().text());
        return false;
    }
    return true;
}
bool MySqlDataBase::exec(const QString &query)
{
    if(!m_sqlQuery.exec(QString(query)))
    {
        setLastError(m_sqlQuery.lastError().text());
        return false;
    }
    return true;
}
bool MySqlDataBase::exec()
{
    if(!m_sqlQuery.exec())
    {
        setLastError(m_sqlQuery.lastError().text());
        return false;
    }
    return true;
}

QMap<QString,QString> MySqlDataBase::getTableInfo(QString table)
{
    QMap<QString,QString>tableMap;
    QString str = QString("PRAGMA table_info(%1)").arg(table);

    m_sqlQuery.prepare(str);

    if(m_sqlQuery.exec())
    {
        while(m_sqlQuery.next())
        {
            tableMap[m_sqlQuery.value(1).toString()] = m_sqlQuery.value(2).toString();
        }
        return tableMap;
    }
    else
    {
        setLastError(m_sqlQuery.lastError().text());
        return tableMap;
    }
}

#if 0
bool MySqlDataBase::initDataBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");//设置数据库类型

    db.setHostName("localhost");//设置数据库主机名
    db.setDatabaseName("mysql");//设置数据库的名字

    db.setUserName("root");//设置数据库登录用户
    db.setPassword("root");//设置数据库登录密码


    if(!db.open())
    {
        qDebug()<<"database open failed";
        QSqlError error = db.lastError();
        qDebug()<<error.text();
        return false;
    }
    qDebug()<<"database open success";
    return true;
}
#endif
