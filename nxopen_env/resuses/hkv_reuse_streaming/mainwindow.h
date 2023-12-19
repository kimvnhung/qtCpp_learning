#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QScopedPointer>
#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
signals:
    void testSignal();
private:
    QScopedPointer<QGraphicsScene> m_scene;
    QScopedPointer<QGraphicsView> m_view;

    QBoxLayout* m_globalLayout = nullptr;
};

#endif // MAINWINDOW_H
