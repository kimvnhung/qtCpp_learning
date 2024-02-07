#include "playcommand.h"

RtspCommand* PlayCommand::createNew(QString url, int CSeq, QString sessionID)
{
    return new PlayCommand(url,CSeq,sessionID);
}

PlayCommand::PlayCommand(QString url,int CSeq,QString sessionID):
    RtspCommand(RtspCommand::PLAY,url,CSeq),
    m_sessionID(sessionID)
{

}

QString PlayCommand::getEntireMessage()
{
    return QString("PLAY %1 RTSP/1.0\r\nCSeq: %2\r\nSession: %3\r\n\r\n")
        .arg(m_url)
        .arg(m_CSeq)
        .arg(m_sessionID);
}

