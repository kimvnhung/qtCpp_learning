#include "result.h"

Result::Result():
    errorCode(ErrorCode::noError)
{
}

Result::Result(
    ErrorCode::Value _errorCode,
    const QString& param1,
    const QString& param2)
    :
    errorCode(_errorCode)
{
    errorParams.push_back(param1);
    errorParams.push_back(param2);
}

Result::operator bool() const
{
    return errorCode == ErrorCode::noError;
}

MediaStreamEvent Result::toMediaStreamEvent() const
{
    switch (errorCode)
    {
    case ErrorCode::tooManyOpenedConnections:
        return MediaStreamEvent::tooManyOpenedConnections;
    case ErrorCode::canNotDecryptMedia:
        return MediaStreamEvent::cannotDecryptMedia;
    default:
        return MediaStreamEvent::noEvent;
    }
}

QString Result::urlToStr(const QUrl& value) const
{
    return value.toString(QUrl::RemoveUserInfo);
}
