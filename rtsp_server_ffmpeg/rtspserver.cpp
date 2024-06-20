#include "rtspserver.h"
#include <QDebug>
#include <QDateTime>

#define RTP_PAYLOAD_TYPE 96 // Dynamic payload type for video

RTSPServer::RTSPServer(QObject *parent)
    : QObject(parent),
    server(new QTcpServer(this)),
    streamTimer(new QTimer(this)),
    formatCtx(nullptr),
    codecCtx(nullptr),
    packet(nullptr),
    frame(nullptr),
    videoStreamIndex(-1),
    startTime(0),
    sequenceNumber(0),
    timestamp(0) {

    connect(server, &QTcpServer::newConnection, this, &RTSPServer::onNewConnection);
    connect(streamTimer, &QTimer::timeout, this, &RTSPServer::streamVideo);
}

RTSPServer::~RTSPServer() {
    if (codecCtx) avcodec_free_context(&codecCtx);
    if (formatCtx) avformat_close_input(&formatCtx);
    if (packet) av_packet_free(&packet);
    if (frame) av_frame_free(&frame);
}

bool RTSPServer::start(const QString &address, quint16 port, const QString &inputFile) {
    if (!server->listen(QHostAddress(address), port)) {
        qCritical() << "Failed to start server:" << server->errorString();
        return false;
    }

    // av_register_all();
    avformat_network_init();

    if (avformat_open_input(&formatCtx, inputFile.toStdString().c_str(), NULL, NULL) != 0) {
        qCritical() << "Could not open input file" << inputFile;
        return false;
    }

    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        qCritical() << "Could not find stream information";
        return false;
    }

    videoStreamIndex = av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (videoStreamIndex < 0) {
        qCritical() << "Could not find video stream in the input file";
        return false;
    }

    AVStream *videoStream = formatCtx->streams[videoStreamIndex];
    AVCodecParameters *codecpar = videoStream->codecpar;
    const AVCodec *decoder = avcodec_find_decoder(codecpar->codec_id);
    if (!decoder) {
        qCritical() << "Failed to find codec";
        return false;
    }

    codecCtx = avcodec_alloc_context3(decoder);
    if (!codecCtx) {
        qCritical() << "Failed to allocate codec context";
        return false;
    }

    if (avcodec_parameters_to_context(codecCtx, codecpar) < 0) {
        qCritical() << "Failed to copy codec parameters to decoder context";
        return false;
    }

    if (avcodec_open2(codecCtx, decoder, NULL) < 0) {
        qCritical() << "Failed to open codec";
        return false;
    }

    packet = av_packet_alloc();
    frame = av_frame_alloc();

    startTime = av_gettime();
    streamTimer->start(40);  // Approximately 25 FPS

    qDebug() << "RTSP server started on" << address << ":" << port;
    return true;
}

void RTSPServer::onNewConnection() {
    while (server->hasPendingConnections()) {
        QTcpSocket *client = server->nextPendingConnection();
        clients.append(client);
        connect(client, &QTcpSocket::disconnected, this, &RTSPServer::onClientDisconnected);
        connect(client, &QTcpSocket::readyRead, this, &RTSPServer::onReadyRead);
        qDebug() << "New client connected";
    }
}

void RTSPServer::onClientDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
        qDebug() << "Client disconnected";
    }
}

void RTSPServer::onReadyRead() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        // Handle RTSP commands from client
        QByteArray requestData = client->readAll();
        QString request(requestData);

        qDebug()<<request;

        if (request.startsWith("OPTIONS")) {
            handleOptions(client, request);
        } else if (request.startsWith("DESCRIBE")) {
            handleDescribe(client, request);
        } else if (request.startsWith("SETUP")) {
            handleSetup(client, request);
        } else if (request.startsWith("PLAY")) {
            handlePlay(client, request);
        } else if (request.startsWith("TEARDOWN")) {
            handleTeardown(client, request);
        } else {
            qDebug() << "Unhandled RTSP request:" << request;
        }
    }
}

void RTSPServer::handleOptions(QTcpSocket *client, const QString &request) {
    QString response = "RTSP/1.0 200 OK\r\n"
                       "CSeq: 1\r\n"
                       "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE\r\n"
                       "\r\n";
    client->write(response.toUtf8());
}

void RTSPServer::handleDescribe(QTcpSocket *client, const QString &request) {
    QString response = "RTSP/1.0 200 OK\r\n"
                       "CSeq: 2\r\n"
                       "Content-Base: rtsp://localhost:8554/\r\n"
                       "Content-Type: application/sdp\r\n"
                       "Content-Length: 95\r\n"
                       "\r\n"
                       "v=0\r\n"
                       "o=- 0 0 IN IP4 127.0.0.1\r\n"
                       "s=No Name\r\n"
                       "c=IN IP4 127.0.0.1\r\n"
                       "t=0 0\r\n"
                       "a=tool:libavformat 58.29.100\r\n"
                       "m=video 0 RTP/AVP 96\r\n"
                       "a=rtpmap:96 H264/90000\r\n";
    client->write(response.toUtf8());
}

void RTSPServer::handleSetup(QTcpSocket *client, const QString &request) {
    QStringList requestLines = request.split("\r\n");
    QString transportLine;
    for (const QString &line : requestLines) {
        if (line.startsWith("Transport:")) {
            transportLine = line;
            break;
        }
    }

    // Extract client ports from Transport header
    QStringList transportParams = transportLine.split(";");
    QString clientPortString;
    for (const QString &param : transportParams) {
        if (param.startsWith("client_port=")) {
            clientPortString = param.mid(QString("client_port=").length());
            break;
        }
    }

    QStringList clientPorts = clientPortString.split("-");
    if (clientPorts.size() == 2) {
        rtpClientPort = clientPorts[0].toUShort();
        rtcpClientPort = clientPorts[1].toUShort();
        clientAddress = client->peerAddress();
    }

    QString response = "RTSP/1.0 200 OK\r\n"
                       "CSeq: 3\r\n"
                       "Transport: RTP/AVP;unicast;client_port=1234-1235\r\n"
                       "Session: 12345678\r\n"
                       "\r\n";
    client->write(response.toUtf8());
}

void RTSPServer::handlePlay(QTcpSocket *client, const QString &request) {
    QString response = "RTSP/1.0 200 OK\r\n"
                       "CSeq: 4\r\n"
                       "Range: npt=0.000-\r\n"
                       "Session: 12345678\r\n"
                       "\r\n";
    client->write(response.toUtf8());
}

void RTSPServer::handleTeardown(QTcpSocket *client, const QString &request) {
    QString response = "RTSP/1.0 200 OK\r\n"
                       "CSeq: 5\r\n"
                       "Session: 12345678\r\n"
                       "\r\n";
    client->write(response.toUtf8());
    client->close();
}

void RTSPServer::streamVideo() {
    if (av_read_frame(formatCtx, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecCtx, packet) == 0) {
                while (avcodec_receive_frame(codecCtx, frame) == 0) {
                    // Synchronize frame presentation based on PTS and real-time
                    int64_t currentTime = av_gettime() - startTime;
                    int64_t framePts = frame->pts * av_q2d(formatCtx->streams[videoStreamIndex]->time_base) * 1000000;
                    int64_t delay = framePts - currentTime;
                    if (delay > 0) {
                        av_usleep(delay);
                    }

                    // Packetize and send the frame using RTP
                    for (QTcpSocket *client : clients) {
                        if (client->state() == QTcpSocket::ConnectedState) {
                            // Packetize the frame into RTP packets
                            sendRTPPacket(rtpSocket, clientAddress, rtpClientPort, frame->data[0], frame->linesize[0], framePts / 90);  // Convert to 90kHz clock
                        }
                    }
                    // qDebug() << "Sent frame PTS:" << frame->pts << "Real time:" << currentTime;
                }
            }
        }
        av_packet_unref(packet);
    }
}

void RTSPServer::sendRTPPacket(QUdpSocket *socket, const QHostAddress &address, quint16 port, uint8_t *data, int len, uint32_t timestamp) {
    static const int RTP_HEADER_SIZE = 12;
    static const int MAX_RTP_PAYLOAD_SIZE = 1400; // Adjust for your network MTU

    uint8_t rtpPacket[RTP_HEADER_SIZE + MAX_RTP_PAYLOAD_SIZE];
    memset(rtpPacket, 0, RTP_HEADER_SIZE);

    // RTP Header
    rtpPacket[0] = 0x80; // Version 2
    rtpPacket[1] = RTP_PAYLOAD_TYPE; // Payload type (dynamic)
    rtpPacket[2] = (sequenceNumber >> 8) & 0xFF; // Sequence number (high byte)
    rtpPacket[3] = sequenceNumber & 0xFF; // Sequence number (low byte)
    rtpPacket[4] = (timestamp >> 24) & 0xFF; // Timestamp (high byte)
    rtpPacket[5] = (timestamp >> 16) & 0xFF; // Timestamp
    rtpPacket[6] = (timestamp >> 8) & 0xFF; // Timestamp
    rtpPacket[7] = timestamp & 0xFF; // Timestamp (low byte)
    rtpPacket[8] = 0x00; // SSRC (high byte)
    rtpPacket[9] = 0x00; // SSRC
    rtpPacket[10] = 0x00; // SSRC
    rtpPacket[11] = 0x01; // SSRC (low byte)

    sequenceNumber++;

    int payloadSize = len;
    if (payloadSize > MAX_RTP_PAYLOAD_SIZE) {
        payloadSize = MAX_RTP_PAYLOAD_SIZE;
    }

    memcpy(rtpPacket + RTP_HEADER_SIZE, data, payloadSize);

    socket->writeDatagram((char*)rtpPacket, RTP_HEADER_SIZE + payloadSize, address, port);
}

void RTSPServer::onRTCPReadyRead() {
    while (rtcpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(rtcpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        rtcpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        // Handle RTCP packet
        qDebug() << "Received RTCP packet from" << sender << ":" << senderPort;
        // Parse and process RTCP packet
    }
}
