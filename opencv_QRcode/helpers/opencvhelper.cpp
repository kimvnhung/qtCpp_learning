#include "opencvhelper.h"

OpenCVHelper::OpenCVHelper(QObject *parent) : QObject(parent)
{

}


OpenCVHelper::~OpenCVHelper()
{

}

void OpenCVHelper::initImage()
{

}

cv::Mat OpenCVHelper::convertQImage2Mat(const QImage &inImage, bool inCloneImageData)
{
    switch (inImage.format()) {
    // 8-bit, 4 channel with alpha
    case QImage::Format_ARGB32: {
        cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar *>(inImage.bits()), inImage.bytesPerLine());
        return (inCloneImageData ? mat.clone() : mat);
    }
    // 8-bit, 4 channel
    case QImage::Format_RGB32: {
        cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar *>(inImage.bits()), inImage.bytesPerLine());
        return (inCloneImageData ? mat.clone() : mat);
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888: {
        if (!inCloneImageData)
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

        QImage swapped = inImage.rgbSwapped();

        return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar *>(swapped.bits()), swapped.bytesPerLine()).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Indexed8: {
        cv::Mat mat(inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar *>(inImage.bits()), inImage.bytesPerLine());

        return (inCloneImageData ? mat.clone() : mat);
    }

    case QImage::Format_ARGB32_Premultiplied: {
        cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar *>(inImage.bits()), inImage.bytesPerLine());
        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}

QImage OpenCVHelper::convertMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    return QImage();
}

QPoint OpenCVHelper::checkPoint(cv::Mat src_img,cv::Mat temp,bool isGetCenter)
{
    cv::Mat template_img;
    cv::Mat result_mat;
    cv::Mat gray_img;


    temp.copyTo(template_img);
    cv::cvtColor(template_img,template_img,cv::COLOR_BGR2GRAY);
    if (template_img.data == NULL) {
        printf("cv::imread() failed...\n");
        return QPoint(-1,-1);
    }

    if (src_img.data == NULL) {
        printf(" %d :screen empty\n",1);
        return QPoint(-1,-1);
    }
    //    qDebug()<<src_img.cols<<","<<src_img.rows;
    //    cv::imshow("temp source",temp);
    //    cv::imshow("temp",template_img);
    //    cv::imshow("screen",src_img);
    //    cv::waitKey(1);
    cv::cvtColor(src_img,src_img,cv::COLOR_BGR2GRAY);


    // method: CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM _CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
    int match_method = cv::TM_SQDIFF_NORMED;

    cv::matchTemplate(src_img, template_img, result_mat, match_method);

    double minVal; double maxVal;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    if( match_method  == cv::TM_SQDIFF
        || match_method == cv::TM_SQDIFF_NORMED ){
        matchLoc = minLoc;
        if(minVal > 0.01){
            return QPoint(-1,-1);
        }
    }else{
        matchLoc = maxLoc;
        if(maxVal < 0.01){
            return QPoint(-1,-1);
        }
    }

    //qDebug()<<"matchLoc :"<<matchLoc.x<<","<<matchLoc.y;
    if(isGetCenter){
        return QPoint(matchLoc.x+template_img.cols/2,matchLoc.y+template_img.rows/2);
    }
    return QPoint(matchLoc.x,matchLoc.y);
}

QList<QPoint> OpenCVHelper::checkPoints(cv::Mat src_img, cv::Mat temp, bool isGetCenter,double offset)
{
    cv::Mat template_img;
    cv::Mat result_mat;
    cv::Mat gray_img;

    QList<QPoint> rt = QList<QPoint>();

    temp.copyTo(template_img);
    if (template_img.data == NULL) {
        printf("cv::imread() failed...\n");
        return rt;
    }
    //    cv::imshow("temp",temp);

    if (src_img.data == NULL) {
        printf(" %d :screen empty\n",1);
        return rt;
    }


    // method: CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM _CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
    int match_method = cv::TM_SQDIFF_NORMED;

    cv::matchTemplate(src_img, template_img, result_mat, match_method);

    double minVal; double maxVal; double matchVal;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    if( match_method  == cv::TM_SQDIFF
        || match_method == cv::TM_SQDIFF_NORMED ){
        matchLoc = minLoc;
        matchVal = minVal;
    }else{
        matchLoc = maxLoc;
        matchVal = maxVal;
    }

    int ignoreCount = 0;
    for(int col = 0; col <result_mat.cols;col++){
        for(int line = 0;line<result_mat.rows;line++){
            cv::Point tempLoc;
            tempLoc.x = col;
            tempLoc.y = line;
            if(abs((result_mat.at<float>(tempLoc)*100-matchVal*100)) <= 0.1){
                //                cv::rectangle(
                //                        src_img,
                //                        tempLoc,
                //                        cv::Point(tempLoc.x+template_img.cols, tempLoc.y+template_img.rows),
                //                        CV_RGB(255,0,0),
                //                        2);
                if(isGetCenter){
                    matchLoc.x = tempLoc.x+template_img.cols/2;
                    matchLoc.y = tempLoc.y+template_img.rows/2;
                }else{
                    matchLoc = tempLoc;
                }

                //clean
                for(int cleanId = 0;cleanId<rt.size();cleanId++){
                    QPoint para = rt.at(cleanId);
                    if(abs(para.x()-matchLoc.x) <= offset
                        && abs(para.y()-matchLoc.y) <= offset){

                        ignoreCount++;
                        break;
                    }
                    if(cleanId == rt.size()-1){
                        rt.append(QPoint(matchLoc.x,matchLoc.y));
                    }
                }
                //add first
                if(rt.size() == 0){
                    rt.append(QPoint(matchLoc.x,matchLoc.y));
                }

            }
        }
    }
    //    cv::Mat resized;
    //    cv::resize(src_img,resized,cv::Size(320,(320*src_img.rows)/src_img.cols));
    //    cv::imshow("src_img",resized);
    //    cv::waitKey(1);
    //    qDebug()<<"ignoreCount "<<ignoreCount;
    //    qDebug()<<"matchsize "<<rt.size();
    //clean
    return rt;
}

//slots
void OpenCVHelper::onRequestCheckPoint(cv::Mat src_img, cv::Mat temp)
{
    cv::Mat template_img;
    cv::Mat result_mat;
    cv::Mat gray_img;


    temp.copyTo(template_img);
    cv::cvtColor(template_img,template_img,cv::COLOR_BGR2GRAY);
    if (template_img.data == NULL) {
        printf("cv::imread() failed...\n");
        emit checkPointCompleted(QPoint(-1,-1));
    }

    if (src_img.data == NULL) {
        printf(" %d :screen empty\n");
        emit checkPointCompleted(QPoint(-1,-1));
    }
    //    qDebug()<<src_img.cols<<","<<src_img.rows;
    //    cv::imshow("temp source",temp);
    //    cv::imshow("temp",template_img);
    //    cv::imshow("screen",src_img);
    //    cv::waitKey(1);
    cv::cvtColor(src_img,src_img,cv::COLOR_BGR2GRAY);


    // method: CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM _CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
    int match_method = cv::TM_SQDIFF_NORMED;

    cv::matchTemplate(src_img, template_img, result_mat, match_method);

    double minVal; double maxVal;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    if( match_method  == cv::TM_SQDIFF
        || match_method == cv::TM_SQDIFF_NORMED ){
        matchLoc = minLoc;
        if(minVal > 0.01){
            emit checkPointCompleted(QPoint(-1,-1));
        }
    }else{
        matchLoc = maxLoc;
        if(maxVal < 0.01){
            emit checkPointCompleted(QPoint(-1,-1));
        }
    }

    //qDebug()<<"matchLoc :"<<matchLoc.x<<","<<matchLoc.y;
    emit checkPointCompleted(QPoint(matchLoc.x+template_img.cols/2,matchLoc.y+template_img.rows/2));
    emit checkOriginPointCompleted(QPoint(matchLoc.x,matchLoc.y));
}
