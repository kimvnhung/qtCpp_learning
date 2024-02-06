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
    if(lines.length() > 0){
        QStringList codeExtracts = lines[0].split(" ");
        if(codeExtracts.length() >= 3){
            return Result{(Result::ResultCode)codeExtracts[1].toInt(),response};
        }
    }
    return Result{Result::UNKNOWN,response};
}

RtspCommand::RtspCommand(Command cmd, QString url,int Cseq) :
    m_cmd(cmd),
    m_url(url),
    m_CSeq(Cseq)
{

}



