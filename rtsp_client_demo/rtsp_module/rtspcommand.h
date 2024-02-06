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

    struct Result
    {
        enum ResultCode {
            OK = 200,
            NOT_FOUND = 404,
            SESSION_NOT_FOUND = 454,
            UNKNOWN = 0
        };

        ResultCode code;
        QString data;
    };

    static Result
    extractResult(QString response);

    virtual QString getEntireMessage();
protected:
    RtspCommand(Command cmd,QString url, int CSeq);
    Command m_cmd;
    QString m_url;
    int m_CSeq;
};

#endif // RTSPCOMMAND_H
