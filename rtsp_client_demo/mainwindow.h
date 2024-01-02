#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videowidgetgenerator.h"

#include <QGridLayout>
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

signals:
    void prepareVideo(QVideoWidget *videoWidget);
public slots:
    void on_addNewVideoWidget(QVideoWidget *videoWidget);

private:
    Ui::MainWindow *ui;

    QMutex mutex;
    // QList<QVideoWidget*> videoWidgets;
    // QList<QMediaPlayer*> mediaPlayers;
    VideoWidgetGenerator *videoGenerator = nullptr;
    QGridLayout *gridLayout = nullptr;
    QThread *backgroundThread = nullptr;
};
#endif // MAINWINDOW_H
