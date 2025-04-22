#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QWidget>

#include <authordialog.h>

#include "authordialog.h"
#include "dbfile.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vrheadsettablemodel.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , headsets()
  , authorDialog(new AuthorDialog(this))
  , headsetTableModel(new VRHeadsetTableModel(this))
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //ui->tableView->setModel(headsetTableModel);
}

MainWindow::~MainWindow()
{
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
    db.read(fileName);
    ui->actionFileOpen->setDisabled(true);
    ui->actionFileClose->setEnabled(true);
    ui->actionFileUpdate->setEnabled(true);
    ui->actionFileWrite->setEnabled(true);

    list<VRHeadset> l;
    db.parse(l);

    for (auto &i : l) {
      if (i.getModelName().empty())
        continue;
    }
  }
}

void MainWindow::on_actionFileWrite_triggered() {
  db.write();
  db.reset();

  ui->actionFileOpen->setEnabled(true);
  ui->actionFileClose->setDisabled(true);
  ui->actionFileUpdate->setDisabled(true);
  ui->actionFileWrite->setDisabled(true);
}
