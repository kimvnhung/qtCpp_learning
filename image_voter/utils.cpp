#include "utils.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>

namespace Utils {

FilterConfig::FilterConfig(QString currentPath, const FilterConfig::Tags& defaultTags)
    : m_currentPath(currentPath), m_tags(defaultTags) {}

QString FilterConfig::currentPath() const {
    return m_currentPath;
}

FilterConfig::Tags FilterConfig::getAllTags() {
    return m_tags;
}

void FilterConfig::addTag(QString tag) {
    if (!m_tags.contains(tag))
        m_tags.append(tag);
}

void FilterConfig::deleteTag(QString tag) {
    m_tags.removeAll(tag);
    m_tagValues.remove(tag);  // Remove the associated images as well
}

FilterConfig::Images FilterConfig::getImages(QString tag) {
    if (!m_tagValues.contains(tag))
        return {};
    return m_tagValues.value(tag);
}

void FilterConfig::setImages(Images images, QString tag) {
    if (m_tagValues.contains(tag))
        qDebug() << "Replace existed values";
    m_tagValues[tag] = images;
}

void FilterConfig::addImage(QString image, QString tag) {
    if (!m_tagValues.contains(tag)) {
        Images images = {image};
        m_tagValues[tag] = images;
        return;
    }
    Images images = m_tagValues.value(tag);
    if (images.contains(image))
        return;
    images.append(image);
    m_tagValues[tag] = images;
}

void FilterConfig::deleteImage(QString image, QString tag) {
    if (!m_tagValues.contains(tag))
        return;
    Images images = m_tagValues.value(tag);
    if (!images.contains(image))
        return;
    images.removeAll(image);
    m_tagValues[tag] = images;
}

QString FilterConfig::imagePath(QString tag, QString image) {
    if (!m_tags.contains(tag) || !m_tagValues.value(tag).contains(image))
        return "";
    if (tag == "All")
        return m_currentPath + "/" + image;
    return m_currentPath + "/" + tag + "/" + image;
}

// Serialize the object to a JSON object
QJsonObject FilterConfig::toJson() const {
    QJsonObject jsonObj;

    // Serialize currentPath
    jsonObj["currentPath"] = m_currentPath;

    // Serialize tags
    QJsonArray tagsArray;
    for (const auto& tag : m_tags) {
        tagsArray.append(tag);
    }
    jsonObj["tags"] = tagsArray;

    // Serialize images for each tag
    QJsonObject imagesObject;
    for (auto it = m_tagValues.begin(); it != m_tagValues.end(); ++it) {
        QJsonArray imageArray;
        for (const auto& image : it.value()) {
            imageArray.append(image);
        }
        imagesObject[it.key()] = imageArray;
    }
    jsonObj["images"] = imagesObject;

    return jsonObj;
}

// Deserialize from JSON object
FilterConfig FilterConfig::fromJson(const QJsonObject& jsonObj) {
    FilterConfig config;

    // Deserialize currentPath
    if (jsonObj.contains("currentPath") && jsonObj["currentPath"].isString()) {
        config.m_currentPath = jsonObj["currentPath"].toString();
    }

    // Deserialize tags
    if (jsonObj.contains("tags") && jsonObj["tags"].isArray()) {
        QJsonArray tagsArray = jsonObj["tags"].toArray();
        for (const auto& tagValue : tagsArray) {
            if (tagValue.isString()) {
                config.addTag(tagValue.toString());
            }
        }
    }

    // Deserialize images
    if (jsonObj.contains("images") && jsonObj["images"].isObject()) {
        QJsonObject imagesObject = jsonObj["images"].toObject();
        for (auto it = imagesObject.begin(); it != imagesObject.end(); ++it) {
            if (it.value().isArray()) {
                QJsonArray imageArray = it.value().toArray();
                Images images;
                for (const auto& imgValue : imageArray) {
                    if (imgValue.isString()) {
                        images.push_back(imgValue.toString());
                    }
                }
                config.m_tagValues[it.key()] = images;
            }
        }
    }

    return config;
}

// Export the object to a JSON file
bool FilterConfig::exportToJson(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing!";
        return false;
    }

    QJsonDocument doc(toJson());
    file.write(doc.toJson());
    file.close();
    return true;
}

void FilterConfig::updateImages() {
    // List all image files in currentPath
    QDir dir(m_currentPath);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png";
    QStringList images = dir.entryList(filters, QDir::Files);

    // Add images name to "All" tag
    setImages(images, "All");
}

void FilterConfig::sync() {
    // Replace old file
    QString filePath = m_currentPath + "/config.json";
    if (!exportToJson(filePath))
        qDebug() << "Sync failed";
}

// Import JSON data from a file and populate the object
FilterConfig FilterConfig::importFromJson(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return FilterConfig();  // Return an empty FilterConfig object
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull()) {
        qDebug() << "Failed to parse JSON!";
        return FilterConfig();
    }

    auto ret = fromJson(doc.object());
    ret.m_currentPath = QFileInfo(filePath).absolutePath();

    return ret;
}

bool copyFile2Folder(QString filePath, QString targetFolder) {
    // Ensure the target directory exists, create it if it doesn't
    QDir targetDir(targetFolder);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(".")) {
            qDebug() << "Failed to create target directory.";
            return false;
        }
    }

    // Get the base name of the file (e.g., "file.txt" from "path/to/file.txt")
    QFileInfo sourceFileInfo(filePath);
    QString targetFilePath = targetFolder + QDir::separator() + sourceFileInfo.fileName();

    // If the file already exists in the target folder, remove it
    QFile targetFile(targetFilePath);
    if (targetFile.exists()) {
        if (!targetFile.remove()) {
            qDebug() << "Failed to remove existing file.";
            return false;
        }
    }

    // Copy the file to the target directory
    qDebug() << "filePath:" << filePath << "targetFilePath" << targetFilePath;
    if (QFile::copy(filePath, targetFilePath)) {
        qDebug() << "File copied successfully!";
        return true;
    } else {
        qDebug() << "Failed to copy the file.";
        return false;
    }
}

}  // namespace Utils
