#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

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


void MainWindow::on_mtbConnect_released()
{

}


void MainWindow::on_btnDisconnect_released()
{

}


void MainWindow::on_btnNext_released()
{

}

