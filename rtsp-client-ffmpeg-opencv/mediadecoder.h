#ifndef MEDIADECODER_H
#define MEDIADECODER_H

#include <QObject>

class MediaDecoder : public QObject
{
    Q_OBJECT
public:
    explicit MediaDecoder(QObject *parent = nullptr);

signals:
};

#endif // MEDIADECODER_H
