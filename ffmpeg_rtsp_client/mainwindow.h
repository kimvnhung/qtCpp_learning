#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

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
    void on_decreaseSpeedBtn_clicked();

    void on_stepBackwardBtn_clicked();

    void on_playPauseBtn_clicked();

    void on_stepForwardBtn_clicked();

    void on_increaseSpeedBtn_clicked();

private:
    Ui::MainWindow *ui;
    Player *player = NULL;
    GLWidget *widget = NULL;
};
#endif // MAINWINDOW_H
