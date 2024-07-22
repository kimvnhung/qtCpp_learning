#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawablewidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPointF>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DrawableWidget *drawer = new DrawableWidget(this);
    drawer->setMinimumSize(600,400);
    QPushButton *clearButton = new QPushButton("Clear", this);
    QPushButton *addPointButton = new QPushButton("Add Point", this);
    QPushButton *addLineButton = new QPushButton("Add Line", this);
    QPushButton *addArcButton = new QPushButton("Add Arc", this);

    connect(clearButton, &QPushButton::clicked,[drawer]{
        drawer->clear();
    });
    connect(addPointButton, &QPushButton::clicked,[drawer,this]{
        drawer->moveTo(randomPoint());
    });
    connect(addLineButton, &QPushButton::clicked,[drawer,this]{
        drawer->lineTo(randomPoint());
    });
    connect(addArcButton, &QPushButton::clicked, [drawer,this]{
        drawer->arcTo(randomPoint(),10,rand()%2);
    });

    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(clearButton);
    vLayout->addWidget(addPointButton);
    vLayout->addWidget(addLineButton);
    vLayout->addWidget(addArcButton);

    layout->addWidget(drawer);
    layout->addLayout(vLayout);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPointF MainWindow::randomPoint()
{
    auto width = size().width();
    auto height = size().height();
    return QPointF(rand() % width, rand() % height);
}
