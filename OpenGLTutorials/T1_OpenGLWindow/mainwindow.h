#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();


    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
private:
    QTimer *m_timer = NULL;
};
#endif // MAINWINDOW_H
