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
    IFrame();
    IFrame(int64_t id, FrameType type, int64_t pts);
    virtual ~IFrame() = default;

    int64_t id() const;
    void setId(int64_t newId);

    FrameType type() const;
    void setType(FrameType newType);

    int64_t getPts() const;
    void setPts(int64_t newPts);

private:
    int64_t m_id{-1};
    FrameType m_type{VIDEO};
    int64_t pts = {-1};
};

class VideoFrame : public IFrame
{
public:
    VideoFrame() = default;
    VideoFrame(const VideoFrame* frame);
    VideoFrame(int64_t id, int64_t pts, int width, int height);

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
    AudioFrame() = default;
    AudioFrame(const AudioFrame* frame);
    AudioFrame(int64_t id, int64_t pts, int sampleRate, int channels);

    int sampleRate() const;
    void setSampleRate(int newSampleRate);

    int channels() const;
    void setChannels(int newChannels);

private:
    int m_sampleRate{0};
    int m_channels{0};
};

#endif // IFRAME_H
