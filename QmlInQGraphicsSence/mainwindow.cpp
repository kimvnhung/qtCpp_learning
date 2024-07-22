#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myrectangle.h"

#include "customgraphicsview.h"
#include "graphitemimpl.h"
#include "griditem.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new CustomGraphicsView(this);
    scene = new QGraphicsScene(this);

    view->setScene(scene);



    setCentralWidget(view);

    // scene->addItem(new MyRectangle());
    scene->addItem(new GridItem(10,10));


    for (int i = 0; i < 10; i++) {
        scene->addItem(new GraphItemImpl(rand()%10+1,rand()%10+1));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
