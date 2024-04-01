#ifndef SAMPLE_H
#define SAMPLE_H

#include <QObject>

class Sample : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(int numHands READ numHands WRITE setNumHands NOTIFY numHandsChanged FINAL)
public:
    explicit Sample(QObject *parent = nullptr);


    int speed() const;
    void setSpeed(int newSpeed);

    int numHands() const;
    void setNumHands(int numHands);

signals:

    void speedChanged();
    void numHandsChanged();

private:
    int m_speed;

    int m_numhands;

};

#endif // SAMPLE_H
