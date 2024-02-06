#include "rtspclient.h"

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

void RtspClient::sendCommand(RtspCommand::Command cmd)
{
    if(m_sessionSocket->isOpen()){
        RtspCommand newCmd = RtspCommand::createNew(cmd,m_url.toDisplayString(),m_CSeq++);
        qDebug()<<__FUNCTION__<<"Write : "<<newCmd.getEntireMessage();
        m_sessionSocket->write(newCmd.getEntireMessage().toUtf8());
        m_sessionSocket->flush();
    }
}

//slots:
void RtspClient::onDataReady()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    RtspCommand::Result res = RtspCommand::extractResult(m_sessionSocket->readAll());
    // m_sessionSocket->close();
    if(res == RtspCommand::OK){
        switch (m_CSeq-1) {
        case RtspCommand::OPTIONS:
            sendCommand(RtspCommand::DESCRIBE);
            break;
        case RtspCommand::DESCRIBE:
            sendCommand(RtspCommand::SETUP);
        case RtspCommand::SETUP:
            sendCommand(RtspCommand::PLAY);
        default:
            sendCommand(RtspCommand::TEARDOWN);
            break;
        }

    }else {
        qDebug()<<"RtspCommand Result : "<<res<<"\n";
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
    sendCommand(RtspCommand::OPTIONS);
}

void RtspClient::onSessionExpired()
{
    qDebug()<<__FUNCTION__<<__LINE__;
}
