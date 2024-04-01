#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "demoquickwidget.h"
#include "sampleview.h"

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

private:
    Ui::MainWindow *ui;
    // DemoQuickWidget *m_demoWidget = NULL;
    SampleView *m_sampleView = NULL;
};
#endif // MAINWINDOW_H
