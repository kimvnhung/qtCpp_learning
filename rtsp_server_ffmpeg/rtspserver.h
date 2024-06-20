#ifndef RTSPSERVER_H
#define RTSPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
}

class RTSPServer : public QObject {
    Q_OBJECT
public:
    explicit RTSPServer(QObject *parent = nullptr);
    ~RTSPServer();

    bool start(const QString &address, quint16 port, const QString &inputFile);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();
    void onRTCPReadyRead();

private:
    void handleClient(QTcpSocket *client);
    void streamVideo();
    void sendRTPPacket(QUdpSocket *socket, const QHostAddress &address, quint16 port, uint8_t *data, int len, uint32_t timestamp);

    void handleOptions(QTcpSocket *client, const QString &request);
    void handleDescribe(QTcpSocket *client, const QString &request);
    void handleSetup(QTcpSocket *client, const QString &request);
    void handlePlay(QTcpSocket *client, const QString &request);
    void handleTeardown(QTcpSocket *client, const QString &request);

    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QTimer *streamTimer;

    AVFormatContext *formatCtx;
    AVCodecContext *codecCtx;
    AVPacket *packet;
    AVFrame *frame;
    int videoStreamIndex;
    int64_t startTime;

    uint16_t sequenceNumber;
    uint32_t timestamp;

    QUdpSocket *rtpSocket;
    QUdpSocket *rtcpSocket;
    QHostAddress clientAddress;
    quint16 rtpClientPort;
    quint16 rtcpClientPort;
};

#endif // RTSPSERVER_H
