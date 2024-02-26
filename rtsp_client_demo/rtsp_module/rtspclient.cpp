#include "playcommand.h"
#include "rtspclient.h"
#include "setupcommand.h"
#include "startupcommand.h"

#define DEFAULT_RTP_PORT 554


const QByteArray RtspClient::kOptionsCommand = "OPTIONS";
const QByteArray RtspClient::kDescribeCommand = "DESCRIBE";
const QByteArray RtspClient::kSetupCommand = "SETUP";
const QByteArray RtspClient::kPlayCommand = "PLAY";
const QByteArray RtspClient::kSetParameterCommand = "SET_PARAMETER";
const QByteArray RtspClient::kGetParameterCommand = "GET_PARAMETER";
const QByteArray RtspClient::kPauseCommand = "PAUSE";
const QByteArray RtspClient::kTeardownCommand = "TEARDOWN";

RtspClient::RtspClient(QObject *parent)
    : QObject{parent}
{

}

RtspClient::~RtspClient()
{

}

bool RtspClient::isOpened() const
{
    QMutexLocker locker(&m_socketMutex);
    return m_tcpSock && m_tcpSock->isOpen();
}

Result RtspClient::open(const QString &url,qint64 startTime)
{
    // if(m_url != url){
    //     m_url = url;
    //     if(m_tcpSock->isOpen()){
    //         m_tcpSock->close();
    //     }


    //     m_tcpSock->connectToHost(m_url.host(),m_url.port(8554));
    //     return m_tcpSock->waitForConnected();
    // }

    // return false;
    {
        QMutexLocker locker(&m_socketMutex);
        m_tcpSock = new QTcpSocket(this);
        connect(m_tcpSock,&QTcpSocket::connected,this,&RtspClient::onSessionEstablished);
        connect(m_tcpSock,&QTcpSocket::readyRead,this,&RtspClient::onDataReady);
        connect(m_tcpSock,&QTcpSocket::errorOccurred,this, &RtspClient::onError);
        connect(m_tcpSock,&QTcpSocket::disconnected,this,&RtspClient::onSessionExpired);
    }

    QHostAddress host(QUrl(url).host());
    m_tcpSock->connectToHost(host,QUrl(url).port(DEFAULT_RTP_PORT));
    if(isOpened()){
        auto result = sendOptions();
        return result;
    }

    return Result{};
}

bool RtspClient::reopen()
{
    return false;
}

QString RtspClient::getSessionError()
{
    return m_tcpSock->errorString();
}

void RtspClient::sendCommand(RtspCommand *cmd)
{
    if(m_tcpSock->isOpen()){
        qDebug()<<__FUNCTION__<<"Write : "<<cmd->getEntireMessage();
        m_tcpSock->write(cmd->getEntireMessage().toUtf8());
        m_tcpSock->flush();
    }
    delete cmd;
}

//slots:
void RtspClient::onDataReady()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    RtspCommand::Result res = RtspCommand::extractResult(m_tcpSock->readAll());
    // m_sessionSocket->close();
    if(res.code == RtspCommand::Result::OK){
        switch (m_CSeq) {
        case RtspCommand::OPTIONS:
            sendCommand(StartupCommand::createNew(RtspCommand::DESCRIBE,m_url.toDisplayString(),++m_CSeq));
            break;
        case RtspCommand::DESCRIBE:
            m_sdp.parse(res.data);
            sendCommand(SetupCommand::createNew(m_url.toDisplayString(),++m_CSeq,m_sdp));
            break;
        case RtspCommand::SETUP:
        {
            QStringList lines = res.data.split("\r\n", Qt::SkipEmptyParts);

            QString sessionID;

            // Extract session ID and CSeq
            for (const QString& line : lines) {
                if (line.startsWith("Session:")) {
                    QStringList tokens = line.split(';');
                    if (tokens.size() >= 1) {
                        QStringList sesions = tokens[0].split(' ');
                        if (sesions.size() >= 2){
                            sessionID = sesions[1]; // Extract session ID
                        }
                    }
                }
            }
            sendCommand(PlayCommand::createNew(m_url.toDisplayString(),++m_CSeq,sessionID));
            break;
        }
        case RtspCommand::PLAY:
        {

            break;
        }
        default:
            sendCommand(StartupCommand::createNew(RtspCommand::TEARDOWN,m_url.toDisplayString(),++m_CSeq));
            break;
        }

    }else {
        qDebug()<<"RtspCommand Result : "<<res.code<<"\n";
        m_CSeq=1;
    }
}

void RtspClient::onError(QAbstractSocket::SocketError errCode)
{
    qWarning()<<"Socket error: "<< m_tcpSock->errorString();
}

void RtspClient::onSessionEstablished()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    sendCommand(StartupCommand::createNew(RtspCommand::OPTIONS,m_url.toDisplayString(),m_CSeq));
}

void RtspClient::onSessionExpired()
{
    qDebug()<<__FUNCTION__<<__LINE__;
}
