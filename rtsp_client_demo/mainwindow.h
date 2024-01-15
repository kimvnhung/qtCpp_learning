#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rtspclientmanager.h"
#include "videowidget.h"

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
    void onFrameAvailable(Handle handleId,QImage *frame);
private:
    Ui::MainWindow *ui;
    static int NUM_PLAYER;
    RtspClientManager *m_rtspClientManager;
    QMap<Handle,VideoWidget*> m_players;
};
#endif // MAINWINDOW_H
