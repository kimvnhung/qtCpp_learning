#ifndef MODULE_FUNCTION_H
#define MODULE_FUNCTION_H

// #include "rtspclient.h"
#include "h264_video_streamer.h"
#include "live555_client.h"

#include <QDebug>

// RtspClient *rtspcl = nullptr;

void startRtspClient(){
    qDebug()<<__FUNCTION__;
    // rtspcl = new RtspClient();

    // if(!rtspcl->open("rtsp://192.168.1.42:8554/h264ESVideoTest")){
    //     qDebug()<<rtspcl->getSessionError();
    // }
    startLive555Client("rtsp://192.168.1.42:8554/h264ESVideoTest");
}

void testH264VideoStreamer()
{
    start();
}

#endif // MODULE_FUNCTION_H
