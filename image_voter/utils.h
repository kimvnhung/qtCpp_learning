#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QJsonDocument>
#include <QObject>
#include <QMap>

namespace Utils {

class FilterConfig {
    using Image = QString;
    using Images = QVector<QString>;
    using Tag = QString;
    using Tags = QStringList;

public:
    FilterConfig(const Tags& defaultTags = {"All"});

    QString currentPath() const;
    Tags getAllTags();

    void addTag(QString tag);
    void deleteTag(QString tag);

    void setImages(Images images, QString tag);
    void addImage(QString image, QString tag);
    void deleteImage(QString image, QString tag);

    QString imagePath(QString tag, QString image);

    static FilterConfig fromJson(const QJsonObject& jsonObj);
    // Export the object to a JSON file
    bool exportToJson(const QString& filePath) const;

    // Import JSON data from a file and populate the object
    static FilterConfig importFromJson(const QString& filePath);

private:
    QString m_currentPath;
    Tags m_tags;
    QMap<Tag, Images> m_tagValues;

    QJsonObject toJson() const;
};

}

#endif // UTILS_H
