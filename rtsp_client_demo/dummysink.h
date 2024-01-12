#ifndef DUMMYSINK_H
#define DUMMYSINK_H

#include <MediaSession.hh>
#include <MediaSink.hh>
#include <QObject>


class DummySink: public QObject, public MediaSink {
    Q_OBJECT
public:
    static DummySink* createNew(UsageEnvironment& env,
                                MediaSubsession& subsession, // identifies the kind of data that's being received
                                char const* streamId = NULL); // identifies the stream itself (optional)

signals:
    void frameDataAvailable();

private:
    DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
        // called only by "createNew()"
    virtual ~DummySink();

    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                           struct timeval presentationTime, unsigned durationInMicroseconds);

private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

private:
    u_int8_t* fReceiveBuffer;
    MediaSubsession& fSubsession;
    char* fStreamId;
};

#endif // DUMMYSINK_H
