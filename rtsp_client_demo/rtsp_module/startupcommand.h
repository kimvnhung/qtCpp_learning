#ifndef STARTUPCOMMAND_H
#define STARTUPCOMMAND_H

#include "rtspcommand.h"


class StartupCommand : protected RtspCommand
{
public:
    static RtspCommand*
    createNew(Command cmd,QString url, int CSeq);
    QString getEntireMessage() override;
private:
    StartupCommand(Command cmd, QString url,int CSeq);
};

#endif // STARTUPCOMMAND_H
