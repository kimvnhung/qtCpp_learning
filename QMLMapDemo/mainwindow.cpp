#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "widgets/mymap.h"

#include <QGridLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyMap *map = new MyMap(this);
    map->widget()->setMinimumSize(400,300);
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QPushButton("Button"));
    layout->addWidget(map->widget());
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
