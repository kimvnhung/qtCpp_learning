#ifndef RTSPCOMMAND_H
#define RTSPCOMMAND_H

#include <QString>

class RtspCommand
{
public:
    enum Command {//value equals to mCSeq
        OPTIONS = 1,
        DESCRIBE = 2,
        SETUP = 3,
        PLAY = 4,
        PAUSE = 5,
        REDIRECT = 6,
        RECORD = 7,
        TEARDOWN = 8,
        SET_PARAMETER = 9
    };

    enum Result {
        OK = 200,
        NOT_FOUND = 404,
        UNKNOWN = -1
    };

    static RtspCommand
    createNew(Command cmd,QString url, int CSeq);

    static Result
    extractResult(QString response);

    QString getEntireMessage();
private:
    RtspCommand(Command &cmd,QString url, int CSeq);
    Command m_cmd;
    QString m_url;
    int m_CSeq;
};

#endif // RTSPCOMMAND_H
