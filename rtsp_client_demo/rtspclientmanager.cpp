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
    QString mUrl;
};


RtspClientManager::RtspClientManager(QObject *parent) :
    QThread{parent},
    d(new Private())
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

char eventLoopWatchVariable = 0;
Handle RtspClientManager::addClient(const QString &url)
{

    Handle handleId = generateHandle();
    d->mUrl = url;
    return handleId;
}

void RtspClientManager::run()
{
    while (true) {
        if(d->mUrl.size() != 0){
            break;
        }
    }
    openURL(*d->env,QString("RtspClient_%1").arg(1).toStdString().c_str(),d->mUrl.toStdString().c_str());
    d->env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
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

