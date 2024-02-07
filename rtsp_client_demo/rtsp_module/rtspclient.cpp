#include "playcommand.h"
#include "rtspclient.h"
#include "setupcommand.h"
#include "startupcommand.h"

RtspClient::RtspClient(QObject *parent)
    : QObject{parent}
{
    m_sessionSocket = new QTcpSocket(this);
    connect(m_sessionSocket,&QTcpSocket::connected,this,&RtspClient::onSessionEstablished);
    connect(m_sessionSocket,&QTcpSocket::readyRead,this,&RtspClient::onDataReady);
    connect(m_sessionSocket,&QTcpSocket::errorOccurred,this, &RtspClient::onError);
    connect(m_sessionSocket,&QTcpSocket::disconnected,this,&RtspClient::onSessionExpired);
}

RtspClient::~RtspClient()
{

}

bool RtspClient::open(const QString &url)
{
    if(m_url != url){
        m_url = url;
        if(m_sessionSocket->isOpen()){
            m_sessionSocket->close();
        }


        m_sessionSocket->connectToHost(m_url.host(),m_url.port(8554));
        return m_sessionSocket->waitForConnected();
    }

    return false;
}

bool RtspClient::reopen()
{
    return false;
}

QString RtspClient::getSessionError()
{
    return m_sessionSocket->errorString();
}

void RtspClient::sendCommand(RtspCommand *cmd)
{
    if(m_sessionSocket->isOpen()){
        qDebug()<<__FUNCTION__<<"Write : "<<cmd->getEntireMessage();
        m_sessionSocket->write(cmd->getEntireMessage().toUtf8());
        m_sessionSocket->flush();
    }
    delete cmd;
}

//slots:
void RtspClient::onDataReady()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    RtspCommand::Result res = RtspCommand::extractResult(m_sessionSocket->readAll());
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
                    QStringList tokens = line.split(' ');
                    if (tokens.size() >= 2) {
                        sessionID = tokens[1]; // Extract session ID
                    }
                }
            }
            sendCommand(PlayCommand::createNew(m_url.toDisplayString(),++m_CSeq,sessionID));
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
    qWarning()<<"Socket error: "<< m_sessionSocket->errorString();
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
