#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H

#include "rtspcommand.h"
#include "sdpparser.h"
#include "result.h"

#include <QMutex>
#include <QObject>
#include <QTcpSocket>
#include <QUrl>

class RtspClient : public QObject
{
    Q_OBJECT
public:
    explicit RtspClient(QObject *parent = nullptr);
    ~RtspClient();

    static const QByteArray kOptionsCommand;
    static const QByteArray kDescribeCommand;
    static const QByteArray kSetupCommand;
    static const QByteArray kPlayCommand;
    static const QByteArray kSetParameterCommand;
    static const QByteArray kGetParameterCommand;
    static const QByteArray kPauseCommand;
    static const QByteArray kTeardownCommand;

    /**
     * @return session connection status
     *
    */
    bool isOpened() const;
    Result open(const QString &url,qint64 startTime);

    Result sendOptions();
    Result sendDescribe();
    bool sendSetup();
    bool sendPlay(qint64 startPos, qint64 endPos, double scale);
    bool sendPause();
    bool sendSetParameter(const QByteArray& paramName, const QByteArray& paramValue);
    bool sendTeardown();

    //old functions

    QString getSessionError();
signals:
private:
    //
    QMutex m_socketMutex;
    QTcpSocket *m_tcpSock = NULL;

    //old attributes
    QUrl m_url;
    int m_CSeq = 1;
    Sdp m_sdp;

    void sendCommand(RtspCommand *cmd);
    bool reopen();

private slots:
    void onDataReady();
    void onError(QAbstractSocket::SocketError errorCode);
    void onSessionEstablished();
    void onSessionExpired();
};

#endif // RTSPCLIENT_H
