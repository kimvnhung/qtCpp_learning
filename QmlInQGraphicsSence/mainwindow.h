#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "previewwidget.h"

#include <QGraphicsView>
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
public slots:
    void onRightMousePressed();
    void onHoverEnter();
    void onHoverLeave();
private:
    Ui::MainWindow *ui;
    QGraphicsView *view = NULL;
    QGraphicsScene *scene = NULL;
    QList<PreviewWidget*> listPreviews;
};
#endif // MAINWINDOW_H
