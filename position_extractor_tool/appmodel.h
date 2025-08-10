#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QSize>

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString imageFolder READ imageFolder NOTIFY imagePathChanged)
    Q_PROPERTY(QSize imageSize READ imageSize NOTIFY imagePathChanged)
public:
    explicit AppModel(QObject *parent = nullptr);

    QString imagePath() const;
    void setImagePath(const QString &path);
    QSize imageSize() const ;

    QString imageFolder() const;

signals:
    void imagePathChanged();
private:
    QString m_imagePath;
    QSize m_imageSize;
};

#endif // APPMODEL_H
