#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AudioThread.h"
#include <SDL2/SDL.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_audioButton_clicked()
{
//    this->audioThead = new AudioThead(this);
//    audioThead->start();
}


void MainWindow::on_playerButton_clicked()
{
    SDL_version version;
    SDL_VERSION(&version);
    qDebug() << version.major << version.minor << version.patch;
}

