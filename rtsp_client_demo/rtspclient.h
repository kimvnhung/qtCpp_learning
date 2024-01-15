#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H



#include "streamclientstate.h"

#include <QObject>
#include <QRunnable>
#include <RTSPClient.hh>

class RTSPClientImpl : public QObject, public QRunnable, public RTSPClient
{
    Q_OBJECT
public:
    static RTSPClientImpl* createNew(UsageEnvironment& env,QObject *parent = nullptr,  const QString &url = "");

    StreamClientState scs;

    void run() override;
signals:
    void availableForDecode(const char* frame);
    void decodeSuccessfull(QImage image);
protected:
    RTSPClientImpl(UsageEnvironment& env, char const* rtspURL,
                  int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum, QObject *parent = nullptr);
    virtual ~RTSPClientImpl();
private:
    class Private;
    QScopedPointer<Private> d;

    void openURL();
    void setupNextSubsession(RTSPClient* rtspClient);
    static void DESCRIBEHandler(RTSPClient* rtspClient,int resultCode, char* resultString);
};

#endif // RTSPCLIENT_H
