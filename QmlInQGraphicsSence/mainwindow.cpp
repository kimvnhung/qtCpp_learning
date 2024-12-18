#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "myrectangle.h"

#include "customgraphicsview.h"
#include "graphitemimpl.h"
#include "griditem.h"
#include "menuwidget.h"
#include "previewgraphicsvideoitem.h"
#include "previewwidget.h"

#include <QGraphicsProxyWidget>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new CustomGraphicsView(this);
    scene = new QGraphicsScene(this);

    view->setScene(scene);


    QVBoxLayout *vLayout = new QVBoxLayout();
    // auto menuWidget = new MenuWidget();
    vLayout->addWidget(view);
    QWidget *central = new QWidget();
    central->setLayout(vLayout);
    setCentralWidget(central);

    // scene->addItem(new MyRectangle());
    scene->addItem(new GridItem(10,10));


    // scene->addWidget(menuWidget->widget());

    QList<GraphItemImpl*> items;
    for (int i = 0; i < 10; i++) {
        auto cell = new GraphItemImpl(rand()%10+1,rand()%10+1);
        if(items.size() == 0)
            items.append(cell);
        else
            for(int j=0;j<items.size();j++)
                if(items[j]->xUnit() == cell->xUnit() && items[j]->yUnit() == cell->yUnit())
                {
                    i--;
                    cell = NULL;
                    break;
                }
                else if(j == items.size()-1)
                {
                    items.append(cell);
                    break;
                }

        if(cell != NULL)
        {
            connect(cell,&GraphItemImpl::rightMousePressed,this,&MainWindow::onRightMousePressed);
            connect(cell,&GraphItemImpl::hoverEnter,this,&MainWindow::onHoverEnter);
            connect(cell,&GraphItemImpl::hoverLeave,this,&MainWindow::onHoverLeave);
            scene->addItem(cell);
        }
    }

    player = new QMediaPlayer(this);
    player->setSource(QUrl("rtsp://admin:admin123@192.168.1.88/live/ch0"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRightMousePressed()
{
    auto item = qobject_cast<GraphItemImpl*>(sender());
    if(item)
    {
        qDebug()<<__FUNCTION__;
        auto menuItem = new MenuWidget();

        QGraphicsProxyWidget *proxy = scene->addWidget(menuItem->widget());
        proxy->setParentItem(item);
        proxy->setPos(item->boundingRect().center());
        proxy->setZValue(1);

        connect(menuItem,&MenuWidget::menuItemClicked,[item,this,proxy](int index){
            if(index == 0)
                item->changeColor();
            else
                item->switchShowCordinate();

            scene->removeItem(proxy);
        });
    }
}

void MainWindow::onHoverEnter()
{
    auto item = qobject_cast<GraphItemImpl*>(sender());
    if(item)
    {
        auto itemCenter = item->boundingRect().center();
        auto videoOutput = new QGraphicsVideoItem();
        listVideo.append(videoOutput);
        player->setVideoOutput(videoOutput);
        scene->addItem(videoOutput);
        // videoOutput->setParentItem(item);
        videoOutput->setPos(itemCenter);
        // videoOutput->show();


        if(player->playbackState() != QMediaPlayer::PlayingState)
            player->play();

        qDebug()<<__FUNCTION__<<__LINE__;
    }
}

void MainWindow::onHoverLeave()
{
    auto item = qobject_cast<GraphItemImpl*>(sender());
    if(item)
    {
        for(int i =0; i< listPreviews.size(); i++){
            if(listPreviews[i]->xUnit() == item->xUnit()
                && listPreviews[i]->yUnit() == item->yUnit())
            {
                scene->removeItem(listPreviews[i]);
                break;
            }
        }

        // scene->removeItem(videoItem);
        if(player->playbackState() == QMediaPlayer::PlayingState)
            player->stop();

        foreach (QGraphicsVideoItem *videoItem, listVideo) {
            scene->removeItem(videoItem);
            // videoItem->deleteLater();
        }

        foreach (QGraphicsItem *child, item->childItems()) {
            scene->removeItem(child);
        }
    }
}
