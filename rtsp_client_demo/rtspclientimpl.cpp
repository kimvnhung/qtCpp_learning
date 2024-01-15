#include "rtspclientimpl.h"

#include <UsageEnvironment.hh>

// Implementation of "ourRTSPClient":

RTSPClientImpl* RTSPClientImpl::createNew(UsageEnvironment& env, char const* rtspURL,
                                          int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum, FrameListener *listener) {
    return new RTSPClientImpl(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum,listener);
}

RTSPClientImpl::RTSPClientImpl(UsageEnvironment& env, char const* rtspURL,
                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum,FrameListener *listener):
    RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1),
    listener(listener) {
}

RTSPClientImpl::~RTSPClientImpl() {
}
