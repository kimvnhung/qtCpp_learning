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
    Q_PROPERTY(double infoRectWidth READ infoRectWidth NOTIFY mousePositionChanged)
    Q_PROPERTY(double infoRectX READ infoRectX WRITE setInfoRectX NOTIFY mousePositionChanged)
    Q_PROPERTY(double infoRectY READ infoRectY WRITE setInfoRectY NOTIFY mousePositionChanged)
    Q_PROPERTY(QString infoText READ infoText WRITE setInfoText NOTIFY mousePositionChanged)
    Q_PROPERTY(bool isInfoSaved READ isInfoSaved WRITE setIsInfoSaved NOTIFY mousePositionChanged)
public:
    explicit AppModel(QObject *parent = nullptr);

    QString imagePath() const;
    void setImagePath(const QString &path);
    QSize imageSize() const ;

    double infoRectWidth() const;

    double infoRectX() const;
    double infoRectY() const;
    QString infoText() const;

    void setInfoText(const QString &text);
    void setInfoRectX(double x);
    void setInfoRectY(double y);

    bool isInfoSaved() const;
    void setIsInfoSaved(bool saved);

    QString imageFolder() const;

signals:
    void imagePathChanged();
    void mousePositionChanged();
public slots:
    void saveRect();
private:
    QString m_imagePath;
    QSize m_imageSize;
    QString m_textInfo;
    double m_infoRectX;
    double m_infoRectY;
    QString m_savedInfo;
    bool m_isInfoSaved;
};

#endif // APPMODEL_H
