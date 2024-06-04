#include <QCoreApplication>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Show image from images.qrc using imshow
    // Load image from resources
    Mat image = imread(":/images/crop1.bmp");
    if (image.empty()) {
        qDebug() << "Error loading image";
        return -1;
    }
    imshow("windowName", image);
    waitKey(0);
    return a.exec();
}
