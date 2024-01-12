#ifndef STREAMCLIENTSTATE_H
#define STREAMCLIENTSTATE_H

#include <MediaSession.hh>


class StreamClientState {
public:
    StreamClientState();
    virtual ~StreamClientState();

public:
    MediaSubsessionIterator* iter;
    MediaSession* session;
    MediaSubsession* subsession;
    TaskToken streamTimerTask;
    double duration;
};

#endif // STREAMCLIENTSTATE_H
