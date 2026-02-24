#pragma once

#include <QObject>
#include <QTranslator>
#include <QUrl>

class QCoreApplication;
class QQmlApplicationEngine;

class LanguageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    explicit LanguageManager(QCoreApplication *app, QObject *parent = nullptr);

    QString language() const;

    Q_INVOKABLE void setLanguage(const QString &language);
    Q_INVOKABLE void toggleLanguage();

    void setQmlEngine(QQmlApplicationEngine *engine);
    void setMainUrl(const QUrl &url);

signals:
    void languageChanged();

private:
    QString normalizedLanguage(const QString &language) const;
    bool loadAndInstall(const QString &language);

    QCoreApplication *m_app = nullptr;
    QQmlApplicationEngine *m_engine = nullptr;
    QTranslator m_translator;
    QString m_language;
    QUrl m_mainUrl;
};
