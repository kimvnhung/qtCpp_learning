#include "iframe.h"

IFrame::IFrame(int id, FrameType type, int64_t pts)
    : m_id(id), m_type(type), pts(pts)
{
}

int IFrame::id() const
{
    return m_id;
}

void IFrame::setId(int newId)
{
    m_id = newId;
}

IFrame::FrameType IFrame::type() const
{
    return m_type;
}

void IFrame::setType(FrameType newType)
{
    m_type = newType;
}

int64_t IFrame::getPts() const
{
    return pts;
}

void IFrame::setPts(int64_t newPts)
{
    pts = newPts;
}

VideoFrame::VideoFrame(int id, int64_t pts, int width, int height)
    : IFrame(id, IFrame::VIDEO, pts), m_width(width), m_height(height)
{
}

int VideoFrame::width() const
{
    return m_width;
}

void VideoFrame::setWidth(int newWidth)
{
    m_width = newWidth;
}

int VideoFrame::height() const
{
    return m_height;
}

void VideoFrame::setHeight(int newHeight)
{
    m_height = newHeight;
}

AudioFrame::AudioFrame(int id, int64_t pts, int sampleRate, int channels)
    : IFrame(id, IFrame::AUDIO, pts), m_sampleRate(sampleRate), m_channels(channels)
{
}

int AudioFrame::sampleRate() const
{
    return m_sampleRate;
}

void AudioFrame::setSampleRate(int newSampleRate)
{
    m_sampleRate = newSampleRate;
}

int AudioFrame::channels() const
{
    return m_channels;
}

void AudioFrame::setChannels(int newChannels)
{
    m_channels = newChannels;
}
