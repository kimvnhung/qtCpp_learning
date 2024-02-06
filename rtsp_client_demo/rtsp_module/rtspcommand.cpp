#include "rtspcommand.h"

#include <QStringList>
#include <QDebug>

QString USER_AGENT = "rtsp_client_demo";

RtspCommand RtspCommand::createNew(Command cmd,QString url,int CSeq)
{
    return RtspCommand(cmd,url,CSeq);
}

RtspCommand::Result RtspCommand::extractResult(QString response)
{
    qDebug()<<__FUNCTION__<<__LINE__<<response;
    QStringList lines = response.split("\r\n");
    if(lines.length() > 0 && lines[0] == "RTSP/1.0 200 OK"){
        return OK;
    }
    return UNKNOWN;
}

RtspCommand::RtspCommand(Command &cmd, QString url,int Cseq) :
    m_cmd(cmd),
    m_url(url),
    m_CSeq(Cseq)
{

}

QString RtspCommand::getEntireMessage()
{
    switch (m_cmd) {
    case OPTIONS:
        return QString("OPTIONS %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    case DESCRIBE:
        return QString("DESCRIBE %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\nAccept: application/sdp\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    case SETUP:
        return QString("SETUP %1/track1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\nTransport: RTP/AVP;unicast;client_port=62176-62177\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    case PLAY:
        return QString("PLAY %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\nSession: E899678F\r\nRange: npt=0.000-\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    case TEARDOWN:
        return QString("TEARDOWN %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\nSession: E899678F").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    default:
        return "";
    }
}


