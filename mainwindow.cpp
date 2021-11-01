#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

#include <memory>
#include <string>
using namespace std;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(QSettings("hgarden-test.ini", QSettings::IniFormat, parent))
{
    ui->setupUi(this);

    ui->edtURL->setText(std::move(settings.value("url").toString()));
    ui->edtUser->setText(std::move(settings.value("user").toString()));
    ui->edtPwd->setText(std::move(settings.value("pwd").toString()));
    ui->edtSerail->setText(std::move(settings.value("serial").toString()));

    controller.setClientOnDataUpdate([&](const Head::Ptr &head)
    {
      showData(Reference::CLIENT, head);
    });

    controller.setServerOnDataUpdate([&](const Head::Ptr &head)
    {
      showData(Reference::SERVER, head);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnConnect_released()
{
  string &&url = ui->edtURL->text().trimmed().toStdString();
  string &&user = ui->edtUser->text().trimmed().toStdString();
  string &&pwd = ui->edtPwd->text().trimmed().toStdString();
  string &&serial = ui->edtSerail->text().trimmed().toStdString();

  if (url.empty() || user.empty() || pwd.empty() || serial.empty())
  {
    QMessageBox msgBox;
    msgBox.setText("All server fields are amndatory");
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.exec();
  }
  else
  {
    settings.setValue("url", ui->edtURL->text().trimmed());
    settings.setValue("user", ui->edtUser->text().trimmed());
    settings.setValue("pwd", ui->edtPwd->text().trimmed());
    settings.setValue("serial", ui->edtSerail->text().trimmed());
    settings.sync();
    try
    {
      controller.connect(url, user, pwd, serial);
      ui->btnDisconnect->setEnabled(true);
      ui->btnConnect->setEnabled(false);
      ui->btnNext->setEnabled(true);

      ui->edtURL->setEnabled(false);
      ui->edtUser->setEnabled(false);
      ui->edtPwd->setEnabled(false);
      ui->edtSerail->setEnabled(false);
    }
    catch (const runtime_error &e)
    {
      QMessageBox msgBox;
      msgBox.setText(e.what());
      msgBox.setStandardButtons(QMessageBox::Cancel);
      msgBox.setDefaultButton(QMessageBox::Cancel);
      msgBox.exec();
    }
  }
}


void MainWindow::on_btnDisconnect_released()
{
  ui->btnDisconnect->setEnabled(false);
  ui->btnConnect->setEnabled(true);
  ui->btnNext->setEnabled(false);
  ui->edtURL->setEnabled(true);
  ui->edtUser->setEnabled(true);
  ui->edtPwd->setEnabled(true);
  ui->edtSerail->setEnabled(true);
  controller.disconnect();
}


void MainWindow::on_btnNext_released()
{
  qDebug().nospace() << "click";
  try
  {
      controller.next();
  }
  catch (const runtime_error &e)
  {
    QMessageBox msgBox;
    msgBox.setText(e.what());
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.exec();
  }

}

void MainWindow::showData(Reference r, const Head::Ptr head)
{
  QCheckBox *ack;
  QCheckBox *cnk;
  QComboBox *package;
  QTextEdit *txt;

  switch (r)
  {
    case Reference::CLIENT:
      ack = ui->ckbClientAck;
      cnk = ui->ckbClientCnk;
      package = ui->cmbClientPackage;
      txt = ui->txtClient;
      break;
    case Reference::SERVER:
      ack = ui->ckbServerAck;
      cnk = ui->ckbServerCnk;
      package = ui->cmbServerPackage;
      txt = ui->txtServer;
      break;
  }

  ack->setEnabled(head->flags | ACK);
  cnk->setEnabled(head->flags | CKN);

  txt->setText(head->getPayload().c_str());

}

