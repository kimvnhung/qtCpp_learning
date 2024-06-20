#ifndef PLAYER_H
#define PLAYER_H

#include "ui/glwidget.h"
#include <QObject>
#include <QSharedPointer>
#include <QMap>

class Player : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum MediaStatus
    {
        NoMedia,
        LoadedMedia,
        EndOfMedia,
        InvalidMedia
    };

    enum Error
    {
        NoError,
        ResourceError,
        FilterError
    };

    struct ErrorData
    {
        Error error;
        QString message;
    };

    explicit Player(QObject *parent = nullptr);
    ~Player();

    void setSource(const QString &source, QMap<QString, QString> options = QMap<QString, QString>());
    void setVideoOutput(GLWidget *widget);
    State state() const;
public slots:
    void play();
    void pause();
    void stop();
signals:
    void error(ErrorData error);
    void stateChanged(State state);
    void positionChanged(qint64 position);
private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // PLAYER_H
