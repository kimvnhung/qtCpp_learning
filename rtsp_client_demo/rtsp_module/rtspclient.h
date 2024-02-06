#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H

#include "rtspcommand.h"
#include "sdpparser.h"

#include <QObject>
#include <QTcpSocket>
#include <QUrl>

class RtspClient : public QObject
{
    Q_OBJECT
public:
    explicit RtspClient(QObject *parent = nullptr);
    ~RtspClient();

    bool open(const QString &url);
    QString getSessionError();
signals:
private:
    QUrl m_url;
    QTcpSocket *m_sessionSocket = NULL;
    int m_CSeq = 1;
    Sdp m_sdp;

    void sendCommand(RtspCommand cmd);
    bool reopen();

private slots:
    void onDataReady();
    void onError(QAbstractSocket::SocketError errorCode);
    void onSessionEstablished();
    void onSessionExpired();
};

#endif // RTSPCLIENT_H
