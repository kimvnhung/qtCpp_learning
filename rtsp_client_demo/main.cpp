

#include <QApplication>


extern "C" {

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}

#include <libavformat/avformat.h>


int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);

    // QStringList argList = QCoreApplication::arguments();

    // MainWindow w;

    // // Opções de abrir a aplicação: Maximizado ou FullScreen
    // if(argList.contains("--fullscreen")) { w.showFullScreen(); }
    // else if (argList.contains("--maximized")) { w.showMaximized(); }
    // else if (argList.contains("--help")) { std::cout << "Parameters:\n\t"
    //                  "--fullscreen:\tOpen Screen in mode FullScreen\n\t"
    //                  "--maximized:\tOpen Screen in mode Maximized"
    //                  "" << std::endl; exit(0); }
    // else { w.show(); }

    // return a.exec();


    AVFormatContext *context = avformat_alloc_context();

    int video_stream_index;

    int audio_stream_index;



    av_register_all();

    avcodec_register_all();

    avformat_network_init();



    //open rtsp

    avformat_open_input(&context,
                        "rtsp://192.168.1.90/MediaInput/Mpeg4",NULL,NULL);

    avformat_find_stream_info(context,NULL);



    //search video stream

    for(int i =0;i<context->nb_streams;i++)

    {

        if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)

        {

            video_stream_index = i;

        }

        else if(context->streams[i]->codec->codec_type ==
                 AVMEDIA_TYPE_AUDIO)

        {

            audio_stream_index = i;

        }

    }



    AVPacket packet;

    av_init_packet(&packet);



    //open output file

    AVOutputFormat* fmt = av_guess_format(NULL,"test2.avi",NULL);

    AVFormatContext* oc = avformat_alloc_context();

    oc->oformat = fmt;

    avio_open2(&oc->pb, "test.avi", AVIO_FLAG_WRITE,NULL,NULL);



    AVStream* stream=NULL;

    int cnt = 0;

    AVCodec *videoCodec;

    AVCodecContext *videoCodecCtx;

    // Get a pointer to the codec context for the video stream

    videoCodecCtx=context->streams[video_stream_index]->codec;

    // Find the decoder for the video stream

    videoCodec=avcodec_find_decoder(videoCodecCtx->codec_id);



    // Open codec

    avcodec_open(videoCodecCtx, videoCodec);



    if(videoCodec->capabilities & CODEC_CAP_TRUNCATED)

    videoCodecCtx->flags |= CODEC_FLAG_TRUNCATED; /* we do not send
complete frames */



    if ((videoCodecCtx->time_base.den / videoCodecCtx->time_base.num) >
            1000 && videoCodecCtx->time_base.num == 1)

    {

        videoCodecCtx->time_base.num = 1000;

    }



    av_read_play(context);//play RTSP

    while(av_read_frame(context,&packet)>=0 && cnt <100){//read 100 frames

        if(packet.stream_index == video_stream_index){//packet is video

            if(stream == NULL){//create stream in file

                stream =
                    avformat_new_stream(oc,context->streams[video_stream_index]->codec->codec);


                avcodec_copy_context(stream->codec,context->streams[video_stream_index]->cod
                                                        ec);

                stream->sample_aspect_ratio =
                    context->streams[video_stream_index]->codec->sample_aspect_ratio;

                avformat_write_header(oc,NULL);

            }

            packet.stream_index = stream->id;

            av_write_frame(oc,&packet);

            cnt++;

        }

        else if(packet.stream_index == audio_stream_index)//packet is audio

        {

            //std::cout<<"Audio packet"<<std::endl;

            cnt++;

        }

        av_free_packet(&packet);

        av_init_packet(&packet);

    }

    av_read_pause(context);

    av_write_trailer(oc);

    avio_close(oc->pb);

    avformat_free_context(oc);



    return (EXIT_SUCCESS);
}
