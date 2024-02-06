#ifndef SETUPCOMMAND_H
#define SETUPCOMMAND_H

#include "rtspcommand.h"
#include "sdpparser.h"


class SetupCommand : protected RtspCommand
{
public:
    static RtspCommand
    createNew(QString url, int CSeq,Sdp sdp);
    QString getEntireMessage() override;
private:
    SetupCommand(QString url,int Cseq,Sdp sdp);
    Sdp m_sdp;
};

#endif // SETUPCOMMAND_H
