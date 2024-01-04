#include "rtspclient.h"

#include <QDebug>

RtspClient::RtspClient(QObject *parent, const QString &url)
    :
    QObject{parent},
    mUrl(url)
{

}

void RtspClient::startOpenRtsp()
{
    qDebug()<<__FUNCTION__<<__LINE__;

}

void RtspClient::stopStream()
{
    qDebug()<<__FUNCTION__<<__LINE__;
}
