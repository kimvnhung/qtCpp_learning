#include "setupcommand.h"

RtspCommand* SetupCommand::createNew(QString url,int CSeq, Sdp sdp)
{
    return new SetupCommand(url,CSeq,sdp);
}

SetupCommand::SetupCommand(QString url,int CSeq,Sdp sdp):
    RtspCommand(RtspCommand::SETUP,url,CSeq),
    m_sdp(sdp)
{

}

QString SetupCommand::getEntireMessage()
{
    Sdp::Media videoMedia;
    for (int i = 0; i < m_sdp.media.size(); i++) {
        if(m_sdp.media[i].mediaType == Sdp::MediaType::Video){
            videoMedia = m_sdp.media[i];
            break;
        }
    }


    return QString("SETUP %1 RTSP/1.0\r\nCSeq: %2\r\nTransport: %3;unicast;client_port=%4-%5\r\n\r\n")
        .arg(m_url+"/"+videoMedia.control)
        .arg(m_CSeq)
        .arg("RTP/AVP")
        .arg(videoMedia.serverPort)
        .arg(videoMedia.serverPort+1);
}


