#pragma once

#include <QObject>
#include <QImage>
#include <QString>

class FFmpegDecoder : public QObject
{
    Q_OBJECT
public:
    explicit FFmpegDecoder(QObject *parent = nullptr);

public slots:
    void start(const QString &url);

signals:
    void frameReady(const QImage &image);
    void finished();
};
