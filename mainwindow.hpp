#pragma once

#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>
using namespace hgardenpi::protocol;

#include <mosquitto.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mtbConnect_released();

    void on_btnDisconnect_released();

    void on_btnNext_released();

private:
    Ui::MainWindow *ui;
};

