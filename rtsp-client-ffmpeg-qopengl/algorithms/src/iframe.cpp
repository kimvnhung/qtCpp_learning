#include "iframe.h"

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
