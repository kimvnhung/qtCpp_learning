#include "appmodel.h"

#include <QImage>
#include <QSettings>
#include <QUrl>

#define SETTINGS QSettings("MyCompany", "MyApp")
#define GET(key, defaultValue) SETTINGS.value(key, defaultValue).toString()
#define SET(key, value) SETTINGS.setValue(key, value)

#define GET_STR(key, defaultValue) SETTINGS.value(key, defaultValue).toString()

AppModel::AppModel(QObject *parent)
    : QObject{parent}
    , m_imagePath("")
    , m_imageSize(0, 0)
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


