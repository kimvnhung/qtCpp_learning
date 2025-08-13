#include "appmodel.h"

#include <QApplication>
#include <QImage>
#include <QSettings>
#include <QUrl>
#include <QClipboard>
#include <QTimer>

#define SETTINGS QSettings("MyCompany", "MyApp")
#define GET(key, defaultValue) SETTINGS.value(key, defaultValue).toString()
#define SET(key, value) SETTINGS.setValue(key, value)

#define GET_STR(key, defaultValue) SETTINGS.value(key, defaultValue).toString()

AppModel::AppModel(QObject *parent)
    : QObject{parent}
    , m_imagePath("")
    , m_imageSize(0, 0)
    , m_textInfo("")
    , m_infoRectX(0.0)
    , m_infoRectY(0.0)
    , m_savedInfo("")
{
    // Load initial image path from QSetting
    setImagePath(GET_STR("imagePath", ""));
}

QString AppModel::imagePath() const
{
    return m_imagePath;
}

void AppModel::setImagePath(const QString &path)
{
    if (m_imagePath != path) {
        m_imagePath = path;
        SET("imagePath", m_imagePath);
        QString localPath = QUrl(m_imagePath).toLocalFile();
        QImage image(localPath);
        if (!image.isNull()) {
            m_imageSize = image.size();
        }else {
            qDebug() << "Failed to load image:" << m_imagePath;
        }
        emit imagePathChanged();
    }
}

QSize AppModel::imageSize() const
{
    return m_imageSize;
}

QString AppModel::imageFolder() const
{
    return m_imagePath.section('/', 0, -2);
}

double AppModel::infoRectWidth() const
{
    return m_textInfo.length() * 6.5;
}

double AppModel::infoRectX() const
{
    return m_infoRectX; // Example X position, can be modified
}

double AppModel::infoRectY() const
{
    return m_infoRectY; // Example Y position, can be modified
}

QString AppModel::infoText() const
{
    return m_textInfo;
}

void AppModel::setInfoText(const QString &text)
{
    if (m_textInfo != text) {
        m_textInfo = text;

        if(m_textInfo.split(",").length() == 2)
            m_savedInfo = "";

        emit mousePositionChanged();
    }
}

void AppModel::setInfoRectX(double x)
{
    if (m_infoRectX != x) {
        m_infoRectX = x;
        emit mousePositionChanged();
    }
}

void AppModel::setInfoRectY(double y)
{
    if (m_infoRectY != y) {
        m_infoRectY = y;
        emit mousePositionChanged();
    }
}

bool AppModel::isInfoSaved() const
{
    return m_isInfoSaved;
}

void AppModel::setIsInfoSaved(bool saved)
{
    if (m_isInfoSaved != saved) {
        m_isInfoSaved = saved;
        emit mousePositionChanged();
    }
}

void AppModel::saveRect()
{
    QApplication::clipboard()->setText(m_textInfo);
    setIsInfoSaved(true);
}


