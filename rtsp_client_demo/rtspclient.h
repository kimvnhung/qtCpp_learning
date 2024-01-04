#ifndef RTSPCLIENT_H
#define RTSPCLIENT_H

#include <QObject>

class RtspClient : public QObject
{
    Q_OBJECT
public:
    explicit RtspClient(QObject *parent = nullptr, const QString &url = "");
public slots:
    void startOpenRtsp();
    void stopStream();
signals:
    void frameAvailable(uint8_t *frame);
private:
    QString mUrl = "";
};

#endif // RTSPCLIENT_H
