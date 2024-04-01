#include "sample.h"

Sample::Sample(QObject *parent):
    QObject{parent},
    m_speed(0),
    m_numhands(0)
{

}

int Sample::speed() const
{
    return m_speed;
}

void Sample::setSpeed(int newSpeed)
{
    if (m_speed == newSpeed)
        return;
    m_speed = newSpeed;
    emit speedChanged();
}

int Sample::numHands() const
{
    return m_numhands;
}

void Sample::setNumHands(int newNumHands)
{
    if(m_numhands == newNumHands)
        return;

    m_numhands = newNumHands;
    emit numHandsChanged();
}
