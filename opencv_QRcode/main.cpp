#include <QCoreApplication>
#include <QImage>
#include <QDebug>

#include "helpers/opencvhelper.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"Hello";

    cv::Mat inputImage = OpenCVHelper::convertQImage2Mat(QImage(":/images/crop4.bmp"));
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    cv::imshow("hello",grayImage);
    // Read the input image
    // Check if the image is loaded successfully
    if (grayImage.empty()) {
        qDebug() << "Error: Could not read the image." ;
        return -1;
    }

    // Set a threshold value (you may need to adjust this based on your specific image)
    int thresholdValue = 110;

    // Apply thresholding
    cv::Mat binaryImage;
    cv::threshold(grayImage, binaryImage, thresholdValue, 255, cv::THRESH_BINARY);

    // Display the input and binary images
    cv::imshow("Input Image", inputImage);
    cv::imshow("Binary Image", binaryImage);


    QImage result = OpenCVHelper::convertMat2QImage(binaryImage);
    if(result.save(":/images/result.png")){
        qDebug()<<"Save success";
    }
    cv::waitKey(0);

    return a.exec();
}
