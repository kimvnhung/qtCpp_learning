#ifndef PLAYCOMMAND_H
#define PLAYCOMMAND_H

#include "rtspcommand.h"


class PlayCommand : protected RtspCommand
{
public:
    static RtspCommand
    createNew(QString url,int CSeq,QString sessionID);

    QString getEntireMessage() override;
private:
    PlayCommand(QString url,int CSeq,QString sessionID);

    QString m_sessionID;
};

#endif // PLAYCOMMAND_H
