#include "globalqueue.h"
#include <QMutexLocker>
QMutex GlobalQueue::m_mutex;
QQueue<QByteArray> GlobalQueue::m_queue;
GlobalQueue* GlobalQueue::m_globalQueue = NULL;

GlobalQueue::GlobalQueue(QObject *parent) : QObject(parent)
{

}

GlobalQueue* GlobalQueue::getInstance()
{
    if(!m_globalQueue)
    {
        m_globalQueue = new GlobalQueue();
    }
    return m_globalQueue;
}

void GlobalQueue::gEnqueue(const QByteArray& array)
{
    QMutexLocker lock(&m_mutex);
    m_queue.enqueue(array);
}
QByteArray GlobalQueue::gDequeue()
{
    QMutexLocker lock(&m_mutex);
    return m_queue.dequeue();
}
int GlobalQueue::getSize()
{
    QMutexLocker lock(&m_mutex);
    return m_queue.size();
}

void GlobalQueue::clearQueue()
{
    QMutexLocker lock(&m_mutex);
    m_queue.clear();
}
