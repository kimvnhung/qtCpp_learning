#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "log.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    widget = new GLWidget(this);
    widget->setMinimumSize(400,600);

    ui->centralwidget->layout()->addWidget(widget);
    player = new Player(this);
    connect(player, &Player::positionChanged,[this](qint64 position){
        // Convert position to format hh:mm:ss
        int seconds = position/1000;
        int hours = seconds/3600;
        seconds -= hours*3600;
        int minutes = seconds/60;
        seconds -= minutes*60;
        ui->positionLb->setText(QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds));
    });
    connect(player, &Player::stateChanged,[this](Player::State state){
        if(state == Player::PlayingState)
            ui->playPauseBtn->setText("pause");
        else if(state == Player::PausedState)
            ui->playPauseBtn->setText("resume");
        else
            ui->playPauseBtn->setText("play");
    });
    player->setVideoOutput(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_decreaseSpeedBtn_clicked()
{

}


void MainWindow::on_stepBackwardBtn_clicked()
{

}


void MainWindow::on_playPauseBtn_clicked()
{
    if(player->state() == Player::StoppedState){
        // Check if text is valid url
        QUrl url(ui->urlEdt->text());
        if(url.isValid()){
            player->setSource(ui->urlEdt->text()
                              ,{
                                {"rtsp_transport","tcp"},
                                {"preset","ultrafast"},
                               }
                            );
            player->play();
        }else
            DBG("Invalid URL");
    }
}


void MainWindow::on_stepForwardBtn_clicked()
{

}


void MainWindow::on_increaseSpeedBtn_clicked()
{

}

