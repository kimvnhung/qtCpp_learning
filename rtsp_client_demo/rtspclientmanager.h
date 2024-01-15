#ifndef RTSPCLIENTMANAGER_H
#define RTSPCLIENTMANAGER_H

#include "framelistener.h"
#include "framequeue.h"

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include "common.h"

class RtspClientManager : public QThread, protected FrameListener
{
    Q_OBJECT
public:
    explicit RtspClientManager(QObject *parent = nullptr);
    ~RtspClientManager();

    void run() override;
    void onFrameAvailable(const char* frame,int dataSize) override;

    Handle addClient(const QString &url);
    // RtspClient* getClient(Handle handleId);
signals:
    void frameAvailable(Handle handleId, QImage *image);
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
