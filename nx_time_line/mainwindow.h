#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bubbletooltip2.h"
#include "demoquickwidget.h"
#include "sampleview.h"
#include "timemarker.h"
#include "timerplayback.h"

#include <QMainWindow>

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
    void on_increaseSpeedBtn_clicked();

    void on_decreaseSpeedBtn_clicked();

    void on_increaseFanBtn_clicked();

    void on_decreaseFanBtn_clicked();

private:
    Ui::MainWindow *ui;
    DemoQuickWidget *m_demoWidget = NULL;
    SampleView *m_sampleView = NULL;
    BubbleToolTip2 *m_bubble = NULL;
    TimeMarker *m_timeMarker = NULL;
    TimerPlayback *m_timerPlayback = NULL;
};
#endif // MAINWINDOW_H
