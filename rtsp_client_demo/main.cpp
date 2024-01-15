#include "mainwindow.h"
#include "videoplayer.h"

#include <iostream>

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList argList = QCoreApplication::arguments();

    MainWindow w;

    // Opções de abrir a aplicação: Maximizado ou FullScreen
    if(argList.contains("--fullscreen")) { w.showFullScreen(); }
    else if (argList.contains("--maximized")) { w.showMaximized(); }
    else if (argList.contains("--help")) { std::cout << "Parameters:\n\t"
                     "--fullscreen:\tOpen Screen in mode FullScreen\n\t"
                     "--maximized:\tOpen Screen in mode Maximized"
                     "" << std::endl; exit(0); }
    else { w.show(); }

    return a.exec();

    // if (argc != 2) {
    //     fprintf(stderr,"usage: sherlock265 videofile.bin\n");
    //     fprintf(stderr,"The video file must be a raw h.265 bitstream (e.g. HM-10.0 output)\n");
    //     exit(5);
    // }


    // QApplication app(argc, argv);

    // VideoPlayer videoPlayer("D:\\Codes\\qtCpp_learning\\rtsp_client_demo\\libde265\\testdata\\girlshy.h265");
    // videoPlayer.show();

    // return app.exec();

}
