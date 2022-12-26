#include "cdatabaseinterface.h"
#include "mysqldatabase.h"
#include <QMap>
#include <QDebug>
#ifdef UTF8
#pragma execution_character_set("utf-8")
#endif
CDatabaseInterface* CDatabaseInterface::m_DatabaseInterface = NULL;

CDatabaseInterface::CDatabaseInterface(QObject *parent) : QObject(parent)
{
    m_sqlDataBase = new MySqlDataBase();
}

bool CDatabaseInterface::openDB(QString name,const QString &type )
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->openDB(name,type);
    }
    return false;
}

void CDatabaseInterface::closeDB()
{
    if(m_sqlDataBase)
    {
        m_sqlDataBase->closeDB();
    }
}

CDatabaseInterface* CDatabaseInterface::getInstance()
{
    if(!m_DatabaseInterface)
        m_DatabaseInterface = new CDatabaseInterface();
    return m_DatabaseInterface;
}

bool  CDatabaseInterface::createTable(QString table,QMap<QString,QString> map)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->createTable(table,map);
    }
    return false;
}

bool CDatabaseInterface::insertRowTable(QString table,QVariantMap map)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->insertRowTable(table,map);
    }
    return false;
}

bool CDatabaseInterface::deleteRowTable(QString table,QString columName,QString value)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->deleteRowTable(table,columName,value);
    }
    return false;
}

bool CDatabaseInterface::updateRowTable(QString table,QString name,QVariantMap map)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->updateRowTable(table,name,map);
    }
    return false;
}

bool CDatabaseInterface::updateRowTable(QString table,QString targetkey,QString targetValue,QString name,QVariantMap map)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->updateRowTable(table,targetkey,targetValue,name,map);
    }
    return false;
}

bool CDatabaseInterface::sortTable(QString table,QString target)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->sortTable(table,target);
    }
    return false;
}

QList<QVariantMap> CDatabaseInterface::getValues(int page,int pageNum)
{
    QList<QVariantMap> List;
    List.clear();
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->getValues(page,pageNum);
    }
    return List;
}

bool CDatabaseInterface::prepare(const QString& query)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->prepare(query);
    }
    return false;
}
bool CDatabaseInterface::exec(const QString &query)
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->exec(query);
    }
    return false;
}
bool CDatabaseInterface::exec()
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->exec();
    }
    return false;
}

QString CDatabaseInterface::getLastError()
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->lastError();
    }
    else
        return QString("数据库未初始化");
}

int CDatabaseInterface::getValuesSize()
{
    if(m_sqlDataBase)
    {
        return m_sqlDataBase->size();
    }
    return 0;
}
