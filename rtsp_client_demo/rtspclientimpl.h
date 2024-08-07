#ifndef RTSPCLIENTIMPL_H
#define RTSPCLIENTIMPL_H

#include "framelistener.h"
#include "streamclientstate.h"

#include <RTSPClient.hh>


// If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
// "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
// showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
// structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

class RTSPClientImpl: public RTSPClient {
public:
    static RTSPClientImpl* createNew(UsageEnvironment& env, char const* rtspURL,
                                    int verbosityLevel = 0,
                                    char const* applicationName = NULL,
                                    portNumBits tunnelOverHTTPPortNum = 0,
                                    FrameListener *listener = NULL);

protected:
    RTSPClientImpl(UsageEnvironment& env, char const* rtspURL,
                  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum,FrameListener *listener);
    // called only by createNew();
    virtual ~RTSPClientImpl();

public:
    StreamClientState scs;
    FrameListener *listener;
};
#endif // RTSPCLIENTIMPL_H
