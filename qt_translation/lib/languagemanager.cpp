#include "languagemanager.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>

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

    const QString qmPath = QStringLiteral("translations/app_%1.qm").arg(language);

    m_app->removeTranslator(&m_translator);
    const bool loaded = m_translator.load(qmPath);
    if (loaded)
        m_app->installTranslator(&m_translator);

    return loaded;
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
