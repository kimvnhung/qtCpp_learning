#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H

#include <QObject>

class RtspClient : public QObject
{
    Q_OBJECT
public:
    explicit RtspClient(QObject *parent = nullptr);

signals:
};

#endif // RTSPCLIENT_H
