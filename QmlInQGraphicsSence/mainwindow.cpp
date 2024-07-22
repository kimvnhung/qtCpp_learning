#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myrectangle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    view->setScene(scene);
    setCentralWidget(view);

    scene->addItem(new MyRectangle());
}

MainWindow::~MainWindow()
{
    delete ui;
}
