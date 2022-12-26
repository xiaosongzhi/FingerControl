#ifndef GLOBALQUEUE_H
#define GLOBALQUEUE_H

#include <QObject>
#include <QMutex>
#include <QQueue>
class GlobalQueue : public QObject
{
    Q_OBJECT
public:

    static GlobalQueue* getInstance();

    void gEnqueue(const QByteArray&);
    QByteArray gDequeue();
    int getSize();
    void clearQueue();
private:
    explicit GlobalQueue(QObject *parent = nullptr);

    static QMutex m_mutex;
    //全局队列，用于数据的传输
    static QQueue<QByteArray> m_queue;
    static GlobalQueue* m_globalQueue;

};

#endif // GLOBALQUEUE_H
