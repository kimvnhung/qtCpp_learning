#include "rtspclientimpl.h"

#include <UsageEnvironment.hh>

// Implementation of "ourRTSPClient":

RTSPClientImpl* RTSPClientImpl::createNew(UsageEnvironment& env, char const* rtspURL,
                                        int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) {
    return new RTSPClientImpl(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

RTSPClientImpl::RTSPClientImpl(UsageEnvironment& env, char const* rtspURL,
                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) {
}

RTSPClientImpl::~RTSPClientImpl() {
}
