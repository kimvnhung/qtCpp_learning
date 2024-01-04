#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rtspclient.h"

#include <QLabel>
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
public slots:
    void onFrameAvailable(uint8_t *frame);
private:
    Ui::MainWindow *ui;
    static int NUM_PLAYER;
    QLabel *showImage = nullptr;
    RtspClient *rtspClient = nullptr;
    QThread *backgroundThread = nullptr;
};
#endif // MAINWINDOW_H
