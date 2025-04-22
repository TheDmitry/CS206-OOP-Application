#include <QDialog>
#include <QListWidget>
#include <QWidget>

#include <authordialog.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionAuthor_triggered()
{
  auto *dialog = new AuthorDialog();
  dialog->show();
}
