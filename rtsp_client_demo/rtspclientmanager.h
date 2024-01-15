#ifndef RTSPCLIENTMANAGER_H
#define RTSPCLIENTMANAGER_H

#include "framequeue.h"

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include "common.h"

class RtspClientManager : public QObject
{
    Q_OBJECT
public:
    explicit RtspClientManager(QObject *parent = nullptr);
    ~RtspClientManager();

    Handle addClient(const QString &url);
    // RtspClient* getClient(Handle handleId);
signals:
    void frameAvailable(Handle handleId, QImage image);
private:
    class Private;
    QScopedPointer<Private> d;

    QMutex mutex;
    // QMap<Handle,RtspClient*> m_clients;
    QThreadPool *m_clientPool;
    QThreadPool *m_decoderPool;
    FrameQueue *m_frameQueue;


    Handle generateHandle();
};

#endif // RTSPCLIENTMANAGER_H
