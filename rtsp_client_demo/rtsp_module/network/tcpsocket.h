#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class TCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit TCPSocket(QObject *parent = nullptr);

signals:
private:
    QTcpSocket *m_sock = NULL;


};

#endif // TCPSOCKET_H
