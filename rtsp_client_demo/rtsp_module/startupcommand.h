#ifndef STARTUPCOMMAND_H
#define STARTUPCOMMAND_H

#include "rtspcommand.h"


class StartupCommand : protected RtspCommand
{
public:
    static RtspCommand
    createNew()
};

#endif // STARTUPCOMMAND_H
