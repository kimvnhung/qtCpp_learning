#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Qt Console Application";

    return app.exec();
}
