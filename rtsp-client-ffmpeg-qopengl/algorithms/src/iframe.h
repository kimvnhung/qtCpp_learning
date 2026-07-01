#ifndef IFRAME_H
#define IFRAME_H

#include <cstdint>

class IFrame
{
public:
    enum FrameType
    {
        VIDEO,
        AUDIO
    };
    IFrame(int id, FrameType type, int64_t pts);

    int id() const;
    void setId(int newId);

    FrameType type() const;
    void setType(FrameType newType);

    int64_t getPts() const;
    void setPts(int64_t newPts);

private:
    int m_id{-1};
    FrameType m_type{VIDEO};
    int64_t pts = {-1};
};

class VideoFrame : public IFrame
{
public:
    VideoFrame(int id, int64_t pts, int width, int height);
    int width() const;
    void setWidth(int newWidth);

    int height() const;
    void setHeight(int newHeight);

private:
    int m_width{0};
    int m_height{0};
};

class AudioFrame : public IFrame
{
public:
    AudioFrame(int id, int64_t pts, int sampleRate, int channels);
    int sampleRate() const;
    void setSampleRate(int newSampleRate);

    int channels() const;
    void setChannels(int newChannels);
private:
    int m_sampleRate{0};
    int m_channels{0};
};

#endif // IFRAME_H
