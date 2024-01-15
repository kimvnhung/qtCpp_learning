#include "rtspclientmanager.h"
#include "rtspconsumefunction.h"

#include <BasicUsageEnvironment.hh>
#include <QImage>
#include <QUrl>
#include <UsageEnvironment.hh>

class RtspClientManager::Private{
public:
    Private()
    {
        TaskScheduler* scheduler = BasicTaskScheduler::createNew();
        this->env = BasicUsageEnvironment::createNew(*scheduler);
    }

    UsageEnvironment* env;
};


RtspClientManager::RtspClientManager(QObject *parent) :
    QObject{parent}
{
    m_decoderPool = new QThreadPool;
    m_decoderPool->setMaxThreadCount(8);

    m_frameQueue = new FrameQueue(this,20);
}

RtspClientManager::~RtspClientManager()
{
    if(m_decoderPool != nullptr) {
        m_decoderPool->waitForDone();
        delete m_decoderPool;
    }
}

Handle RtspClientManager::addClient(const QString &url)
{

    Handle handleId = generateHandle();
    openURL(*d->env,QString("RtspClient_%1").arg(handleId).toStdString().c_str(),url.toStdString().c_str());
    return handleId;
}

// RtspClient* RtspClientManager::getClient(Handle handleId)
// {
//     QMutexLocker locker(&mutex);
//     return m_clients.value(handleId,nullptr);
// }

Handle RtspClientManager::generateHandle()
{
    // QMutexLocker locker(&mutex);
    // return m_clients.size()+1;
    return 0;
}

