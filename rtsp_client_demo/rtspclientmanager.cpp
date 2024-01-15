#include "framedecoder.h"
#include "rtspclientmanager.h"
#include "rtspconsumefunction.h"

#include <BasicUsageEnvironment.hh>
#include <QImage>
#include <QUrl>
#include <QDebug>
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

    m_frameQueue = new FrameQueue(this,2);

    FrameDecoder *decoder = new FrameDecoder(NULL,m_frameQueue);
    connect(decoder, &FrameDecoder::decodeDone,this,&RtspClientManager::frameAvailable);

    m_decoderPool->start(decoder);
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
    d->mUrl = url;
    return handleId;
}

char eventLoopWatchVariable = 0;
void RtspClientManager::run()
{

    while (true) {
        if(d->mUrl.size() != 0){
            break;
        }
    }
    openURL(*d->env,QString("RtspClient_%1").arg(1).toStdString().c_str(),d->mUrl.toStdString().c_str(),this);
    d->env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
}

void RtspClientManager::onFrameAvailable(const char* frame,int dataSize)
{
    qDebug()<<"onFrameAvailable dataSize : "<<dataSize;
    if(m_frameQueue->isFull()){
        // m_frameQueue->waitForNotFull();
        return;
    }
    m_frameQueue->addFrame(Frame{
        frame,dataSize,0
    });
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

