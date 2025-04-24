#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QWidget>
#include <iostream>

#include <controllers/authordialog.h>

#include "authordialog.h"
#include "mainwindow.h"
#include "models/vrheadsettablemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , authorDialog(new AuthorDialog(this))
  , headsetTableModel(new VRHeadsetTableModel(this))
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->tableView->setModel(headsetTableModel);
}

MainWindow::~MainWindow() {
  delete authorDialog;
  delete headsetTableModel;
  delete ui;
}

void MainWindow::on_actionProgramAuthor_triggered()
{
  authorDialog->exec();
}

void MainWindow::on_actionFileOpen_triggered()
{
  string fileName = QFileDialog::getOpenFileName(this,
                                                 "Read file",
                                                 QDir::currentPath(),
                                                 "Db Files (.db);;All Files (.*)")
                      .toStdString();
  if (!fileName.empty()) {
    headsetTableModel->readFromFile(fileName);

    ui->actionFileOpen->setDisabled(true);
    ui->actionFileClose->setEnabled(true);
    ui->actionFileUpdate->setEnabled(true);
    ui->actionFileWrite->setEnabled(true);
  }
}

void MainWindow::on_actionFileWrite_triggered() {
  headsetTableModel->writeToFile();
}

void MainWindow::on_actionFileUpdate_triggered() {
  headsetTableModel->readFromFile();
}
void MainWindow::on_actionFileClose_triggered() {
  headsetTableModel->reset();

  ui->actionFileOpen->setEnabled(true);
  ui->actionFileClose->setDisabled(true);
  ui->actionFileUpdate->setDisabled(true);
  ui->actionFileWrite->setDisabled(true);
}
