#include "mainwindow.h"

#include "resourcecontainer.h"

#include <QTimer>

static constexpr int kMinimalWindowWidth = 1024;
static constexpr int kMinimalWindowHeight = 768;
static constexpr int kSmallMinimalWindowWidth = 400;
static constexpr int kSmallMinimalWindowHeight = 300;

static constexpr int CONTAINER_COUNTER = 6;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setMinimumWidth(kMinimalWindowWidth);
    setMinimumHeight(kMinimalWindowHeight);

    m_scene.reset(new QGraphicsScene(this));

    m_view.reset(new QGraphicsView(m_scene.data(),this));
    m_view->setAutoFillBackground(true);
    m_view->setAttribute(Qt::WA_DontCreateNativeAncestors);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);

    m_scene->setSceneRect(-300,-300,600,600);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    for (int i = 0; i < 6; ++i) {
        ResourceContainer *container = new ResourceContainer();
        container->setPos(::sin((i * 6.28) / CONTAINER_COUNTER) * 200,
                      ::cos((i * 6.28) / CONTAINER_COUNTER) * 200);
        m_scene->addItem(container);
    }

    QTimer timer;
    connect(&timer, &QTimer::timeout, m_scene.data(), &QGraphicsScene::advance);
    timer.start(5);

    m_globalLayout = new QVBoxLayout(this);
    m_globalLayout->setContentsMargins(0, 0, 0, 0);
    m_globalLayout->setSpacing(0);

    m_globalLayout->addWidget(m_view.data());

    setLayout(m_globalLayout);
}

MainWindow::~MainWindow()
{
    if(m_globalLayout != nullptr){
        m_globalLayout->deleteLater();
    }
}


