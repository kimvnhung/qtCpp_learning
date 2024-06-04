#include <QCoreApplication>
#include <QFile>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Check if qrc file path is correct
    QFile file(":/images/crop1.bmp");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file";
        return -1;
    }

    QByteArray imageData = file.readAll();
    cv::Mat buf(1, imageData.size(), CV_8UC1, imageData.data());
    cv::Mat image = cv::imdecode(buf, cv::IMREAD_COLOR);

    if (image.empty()) {
        qDebug() << "Error loading image";
        return -1;
    }

    imshow("windowName", image);

    waitKey(0);
    return a.exec();
}
