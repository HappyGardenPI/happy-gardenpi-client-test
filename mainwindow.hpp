#pragma once

#include <QMainWindow>
#include <QSettings>

#include "maincontroller.hpp"
using hgarden::test::MainController;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

  QSettings settings;
  MainController controller;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_btnConnect_released();

  void on_btnDisconnect_released();

  void on_btnNext_released();

private:
    Ui::MainWindow *ui;
};

