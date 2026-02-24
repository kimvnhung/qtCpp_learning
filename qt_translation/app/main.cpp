#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/QCoreApplication>
#include <QtCore/QLocale>
#include <QQmlContext>

#include "languagemanager.h"

static QString selectedLanguage()
{
    const QByteArray env = qgetenv("APP_LANG");
    if (!env.isEmpty())
        return QString::fromUtf8(env);

    const QString system = QLocale::system().name();
    if (system.startsWith(QStringLiteral("vi")))
        return QStringLiteral("vi");

    return QStringLiteral("en");
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const QString lang = selectedLanguage().toLower();

    QQmlApplicationEngine engine;

    LanguageManager langManager(&app);
    langManager.setQmlEngine(&engine);
    engine.rootContext()->setContextProperty(QStringLiteral("langManager"), &langManager);

    const QString qmlImportPath = QCoreApplication::applicationDirPath() + QStringLiteral("/../qml");
    engine.addImportPath(qmlImportPath);

    const QUrl mainUrl = QUrl::fromLocalFile(
        qmlImportPath + QStringLiteral("/TranslationLib/qml/MainView.qml"));

    langManager.setMainUrl(mainUrl);
    langManager.setLanguage(lang);

    engine.load(mainUrl);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
