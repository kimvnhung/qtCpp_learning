#ifndef VIDEODECODER_H
#define VIDEODECODER_H




#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui>
#ifdef HAVE_SWSCALE
#ifdef __cplusplus
extern "C" {
#endif
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif
#endif

#include "videowidget.h"
#include "libde265/de265.h"

class VideoDecoder : public QThread
{
    Q_OBJECT

public:
    VideoDecoder();
    ~VideoDecoder();

    // void init(const char* filename);

protected:
    void run();  // thread entry point

public slots:
    void startDecoder();
    void stopDecoder();
    void singleStepDecoder();
    void hasNewFrameBuffer(uint8_t* frame,int frameSize);

    void showCBPartitioning(bool flag);
    void showTBPartitioning(bool flag);
    void showPBPartitioning(bool flag);
    void showIntraPredMode(bool flag);
    void showPBPredMode(bool flag);
    void showQuantPY(bool flag);
    void showMotionVec(bool flag);
    void showTiles(bool flag);
    void showSlices(bool flag);
    void showDecodedImage(bool flag);

signals:
    void displayImage(QImage*);

private:
    // de265 decoder

    // FILE* mFH;
    //input_context_FILE inputctx;
    //rbsp_buffer buf;
    de265_decoder_context* ctx;
    const de265_image* img;

    QMutex mutex;

    QImage mImgBuffers[2];
    int    mNextBuffer;
    int    mFrameCount;

    bool   mPlayingVideo;
    bool   mVideoEnded;
    bool   mSingleStep;


    bool   mShowDecodedImage;
    bool   mShowQuantPY;
    bool   mCBShowPartitioning;
    bool   mTBShowPartitioning;
    bool   mPBShowPartitioning;
    bool   mShowIntraPredMode;
    bool   mShowPBPredMode;
    bool   mShowMotionVec;
    bool   mShowTiles;
    bool   mShowSlices;

    uint8_t* frameBuffer;
    int frameBufferSize;

    void decoder_loop();

    void init_decoder();
    void free_decoder();

    void show_frame(const de265_image* img);
#ifdef HAVE_VIDEOGFX
    void convert_frame_libvideogfx(const de265_image* img, QImage & qimg);
#endif
#ifdef HAVE_SWSCALE
    SwsContext* sws;
    int width;
    int height;
    void convert_frame_swscale(const de265_image* img, QImage & qimg);
#endif
};

#endif // VIDEODECODER_H
