#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QtCore/QCoreApplication>
#include <QtCore/QLocale>

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

    QTranslator translator;
    const QString qmPath = lang.startsWith(QStringLiteral("vi"))
        ? QStringLiteral(":/i18n/qt_translation_vi.qm")
        : QStringLiteral(":/i18n/qt_translation_en.qm");

    if (translator.load(qmPath))
        app.installTranslator(&translator);

    QQmlApplicationEngine engine;

    const QString qmlImportPath = QCoreApplication::applicationDirPath() + QStringLiteral("/../qml");
    engine.addImportPath(qmlImportPath);

    const QUrl mainUrl = QUrl::fromLocalFile(
        qmlImportPath + QStringLiteral("/TranslationLib/qml/MainView.qml"));
    engine.load(mainUrl);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
