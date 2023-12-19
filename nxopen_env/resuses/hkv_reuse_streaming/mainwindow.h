#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QScopedPointer>

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>

#include <QBoxLayout>
#include <QGraphicsView>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    virtual ~MainWindow() override;
signals:
private:
    QScopedPointer<QGraphicsScene> m_scene;
    QScopedPointer<QGraphicsView> m_view;

    QBoxLayout* m_globalLayout = nullptr;
};

#endif // MAINWINDOW_H
