#include "iconsumer.h"

void IConsumer::run()
{
    m_running = true;

    while (m_running)
    {
        renderFrame();
        // Sleep or wait for the next frame based on the playback clock
        msleep(16); // Assuming 60 FPS, adjust as needed
    }
}

QString IConsumer::name() const
{
    return m_name;
}

void IConsumer::setName(const QString &newName)
{
    m_name = newName;
}

std::shared_ptr<IFrame> IConsumer::frame() const
{
    return m_frame;
}

void IConsumer::setFrame(const std::shared_ptr<IFrame> &newFrame)
{
    m_frame = newFrame;
}

void IConsumer::stop()
{
    m_running = false;
}
