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

