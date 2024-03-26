#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ffmpegplayer.h"
#include <QMainWindow>
#include <QMutex>
#include <QQueue>

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

private slots:
    void on_openRGBbtn_clicked();

    void on_openYUVbtn_clicked();

    void on_frameAvailable(char* yuvData, int width, int height, int *stride);

    void on_handleFrame();
private:
    Ui::MainWindow *ui;
    FfmpegPlayer *m_player = NULL;
    QTimer *m_timer = NULL;
    FrameQueue *m_queue = NULL;
};
#endif // MAINWINDOW_H
