#ifndef RTSPCLIENTIMPLEMENTED_H
#define RTSPCLIENTIMPLEMENTED_H

#include "streamclientstate.h"

#include <RTSPClient.hh>


// If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
// "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
// showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
// structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

class RTSPClientImplemented: public RTSPClient {
public:
    static RTSPClientImplemented* createNew(UsageEnvironment& env, char const* rtspURL,
                                    int verbosityLevel = 0,
                                    char const* applicationName = NULL,
                                    portNumBits tunnelOverHTTPPortNum = 0);

protected:
    RTSPClientImplemented(UsageEnvironment& env, char const* rtspURL,
                  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
    virtual ~RTSPClientImplemented();

public:
    StreamClientState scs;
};

#endif // RTSPCLIENTIMPLEMENTED_H
