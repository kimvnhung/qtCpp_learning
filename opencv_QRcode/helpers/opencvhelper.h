#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include <QObject>
#include <QImage>
#include <QDebug>

//opencv2
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;

#include "opencv2/imgproc/imgproc.hpp"


class OpenCVHelper : public QObject
{
    Q_OBJECT
public:
    static OpenCVHelper* getInstance();
    static void initImage();
    static cv::Mat convertQImage2Mat(const QImage &inImage, bool inCloneImageData = true);
    static QImage convertMat2QImage(const cv::Mat& mat);

    static QPoint checkPoint(cv::Mat screen,cv::Mat temp,bool isGetCenter = true);
    static QList<QPoint> checkPoints(cv::Mat screen,cv::Mat temp,bool isGetCenter = true,double offset = 120);
public slots:
    void onRequestCheckPoint(cv::Mat screen,cv::Mat temp);
signals:
    void checkPointCompleted(QPoint result);
    void checkPointsCompleted(QList<QPoint> results);
    void checkOriginPointCompleted(QPoint result);
private:
    static OpenCVHelper* instance;
    explicit OpenCVHelper(QObject *parent = nullptr);
    ~OpenCVHelper();



    //login
    static cv::Mat IMGloadingfacebook;
    static cv::Mat IMGlogin;
    static cv::Mat IMGlogin4;
    static cv::Mat IMGusernameinput;
    static cv::Mat IMGpasswordinput;

    static cv::Mat IMGlogincoderequired;
    static cv::Mat IMGbtnOk;

    static cv::Mat IMGcontinue;
    static cv::Mat IMGlogincodeinput;
    static cv::Mat IMGsavelogininfo;
    static cv::Mat IMGmaininterfaceheader;

    //interaction
    static cv::Mat IMGlikebtn;
    static cv::Mat IMGcommentbtn;

    //errorImage
    static cv::Mat IMGnointernet;
    static cv::Mat IMGaccountdisable;
    static cv::Mat IMGaccountdisable2;
    static cv::Mat IMGincorrectpass;

};

#endif // OPENCVHELPER_H
