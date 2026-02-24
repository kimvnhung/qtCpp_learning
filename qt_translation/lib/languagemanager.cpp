#include "languagemanager.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QTranslator>

LanguageManager::LanguageManager(QCoreApplication *app, QObject *parent)
    : QObject(parent)
    , m_app(app)
{
}

QString LanguageManager::language() const
{
    return m_language;
}

void LanguageManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    m_engine = engine;
}

void LanguageManager::setMainUrl(const QUrl &url)
{
    m_mainUrl = url;
}

QString LanguageManager::normalizedLanguage(const QString &language) const
{
    const QString lang = language.trimmed().toLower();
    if (lang.startsWith(QStringLiteral("vi")))
        return QStringLiteral("vi");
    return QStringLiteral("en");
}

bool LanguageManager::loadAndInstall(const QString &language)
{
    if (!m_app)
        return false;

    const QString sourceQmFile = QStringLiteral("app_%1.qm").arg(language);
    const QString idQmFile = QStringLiteral("app_%1_id.qm").arg(language);
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString parentDir = QDir(appDir).absoluteFilePath(QStringLiteral(".."));

    const QString sourceResourcePath = QStringLiteral(":/i18n/%1").arg(sourceQmFile);
    const QString idResourcePath = QStringLiteral(":/i18n/%1").arg(idQmFile);

    if (m_idTranslator) {
        m_app->removeTranslator(m_idTranslator);
        delete m_idTranslator;
        m_idTranslator = nullptr;
    }
    if (m_sourceTranslator) {
        m_app->removeTranslator(m_sourceTranslator);
        delete m_sourceTranslator;
        m_sourceTranslator = nullptr;
    }

    m_sourceTranslator = new QTranslator(this);
    m_idTranslator = new QTranslator(this);

    bool sourceLoaded = m_sourceTranslator->load(sourceQmFile, appDir);
    if (!sourceLoaded)
        sourceLoaded = m_sourceTranslator->load(sourceQmFile, parentDir);
    if (!sourceLoaded)
        sourceLoaded = m_sourceTranslator->load(sourceQmFile);
    if (!sourceLoaded)
        sourceLoaded = m_sourceTranslator->load(sourceResourcePath);

    bool idLoaded = m_idTranslator->load(idQmFile, appDir);
    if (!idLoaded)
        idLoaded = m_idTranslator->load(idQmFile, parentDir);
    if (!idLoaded)
        idLoaded = m_idTranslator->load(idQmFile);
    if (!idLoaded)
        idLoaded = m_idTranslator->load(idResourcePath);

    if (sourceLoaded)
        m_app->installTranslator(m_sourceTranslator);
    if (idLoaded)
        m_app->installTranslator(m_idTranslator);

    return sourceLoaded || idLoaded;
}

void LanguageManager::setLanguage(const QString &language)
{
    const QString normalized = normalizedLanguage(language);
    if (normalized == m_language)
        return;

    loadAndInstall(normalized);
    m_language = normalized;
    emit languageChanged();

    if (m_engine) {
        if (!QMetaObject::invokeMethod(m_engine, "retranslate")) {
            if (m_mainUrl.isValid()) {
                m_engine->clearComponentCache();
                m_engine->load(m_mainUrl);
            }
        }
    }
}

void LanguageManager::toggleLanguage()
{
    setLanguage(m_language == QStringLiteral("vi") ? QStringLiteral("en") : QStringLiteral("vi"));
}
