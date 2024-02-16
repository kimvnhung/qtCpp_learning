#include "rtspclientimplemented.h"


RTSPClientImplemented* RTSPClientImplemented::createNew(UsageEnvironment& env, char const* rtspUrl,
                                                        int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
{
    return new RTSPClientImplemented(env,rtspUrl,verbosityLevel,applicationName,tunnelOverHTTPPortNum);
}

RTSPClientImplemented::RTSPClientImplemented(UsageEnvironment& env, char const* rtspURL,
                                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) :
    RTSPClient(env,rtspURL,verbosityLevel,applicationName,tunnelOverHTTPPortNum,-1)
{

}

RTSPClientImplemented::~RTSPClientImplemented()
{

}

