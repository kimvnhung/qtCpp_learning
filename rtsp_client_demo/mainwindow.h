#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMutex>
#include <QVideoWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    static int NUM_PLAYER;
    QMutex mutex;

    QList<QVideoWidget*> videoWidgets;
    QList<QMediaPlayer*> mediaPlayers;
    QThread *backgroundThread = nullptr;
};
#endif // MAINWINDOW_H
