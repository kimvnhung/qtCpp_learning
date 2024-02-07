#include "startupcommand.h"

RtspCommand* StartupCommand::createNew(Command cmd,QString url,int CSeq)
{
    return new StartupCommand(cmd,url,CSeq);
}

StartupCommand::StartupCommand(Command cmd, QString url, int CSeq):
    RtspCommand(cmd,url,CSeq)
{

}

QString StartupCommand::getEntireMessage()
{
    if(m_cmd == DESCRIBE){
        return QString("DESCRIBE %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\nAccept: application/sdp\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
    }

    return QString("OPTIONS %1 RTSP/1.0\r\nCSeq: %2\r\nUser-Agent: %3\r\n\r\n").arg(m_url).arg(m_CSeq).arg(USER_AGENT);
}


