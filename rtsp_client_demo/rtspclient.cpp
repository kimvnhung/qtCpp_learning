
#include "rtspclient.h"
#include <BasicUsageEnvironment.hh>
#include <QDebug>

char eventLoopWatchVariable = 0;

// A function that outputs a string that identifies each stream (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient) {
    return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession) {
    return env << subsession.mediumName() << "/" << subsession.codecName();
}

class RTSPClientImpl::Private{
public:
    Private()
    {

    }
    UsageEnvironment* env;

};

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 // by default, print verbose output from each "RTSPClient"

RTSPClientImpl* RTSPClientImpl::createNew(UsageEnvironment& env, QObject *parent, const QString &mUrl)
{
    return new RTSPClientImpl(env,mUrl.toStdString().c_str(),RTSP_CLIENT_VERBOSITY_LEVEL,"RTSPClientImpl",0);
}

RTSPClientImpl::RTSPClientImpl(UsageEnvironment& env, char const* rtspURL,
                               int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum, QObject *parent) :
    QObject(parent),
    RTSPClient(env,rtspURL,verbosityLevel,applicationName,tunnelOverHTTPPortNum,-1),
    d(new Private())
{
    sendDescribeCommand();
}

RTSPClientImpl::~RTSPClientImpl()
{

}

void RTSPClientImpl::run()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    openURL();
    d->env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
}


void RTSPClientImpl::openURL()
{
    d->client = ourRTSPClient::createNew(*d->env,mUrl.toStdString().c_str(),RTSP_CLIENT_VERBOSITY_LEVEL,"RTSPClientImpl");
    if(d->client == nullptr){
        *d->env << "Failed to create a RTSP client for URL \"" << mUrl.toStdString().c_str() << "\": " << d->env->getResultMsg() << "\n";
        return;
    }

    d->client->sendDescribeCommand(DESCRIBEHandler);
}



void RTSPClientImpl::DESCRIBEHandler(RTSPClient* rtspClient,int resultCode, char* resultString)
{
    do {
        UsageEnvironment& env = rtspClient->envir(); // alias
        StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

        if (resultCode != 0) {
            env << *rtspClient << "Failed to get a SDP description: " << resultString << "\n";
            delete[] resultString;
            break;
        }

        char* const sdpDescription = resultString;
        env << *rtspClient << "Got a SDP description:\n" << sdpDescription << "\n";

        // Create a media session object from this SDP description:
        scs.session = MediaSession::createNew(env, sdpDescription);
        delete[] sdpDescription; // because we don't need it anymore
        if (scs.session == NULL) {
            env << *rtspClient << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
            break;
        } else if (!scs.session->hasSubsessions()) {
            env << *rtspClient << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
            break;
        }

        // Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
        // calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
        // (Each 'subsession' will have its own data source.)
        scs.iter = new MediaSubsessionIterator(*scs.session);
        setupNextSubsession(rtspClient);
        return;
    } while (0);

    // An unrecoverable error occurred with this stream.
    shutdownStream(rtspClient);
}



