#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QWidget>

#include <QKeyCombination>
#include <QKeySequence>
#include <QShortcut>

#include "controllers/authordialog.h"
#include "controllers/mainwindow.h"
#include "controllers/workspace.h"
#include "models/customtablemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , authorDialog(new AuthorDialog(this))
  , workspace(new Workspace())
  , workspaceInitialized(false)
  , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  workspace->setHidden(true);

  shortcut["newTab"] = new QShortcut(QKeyCombination(Qt::CTRL, Qt::Key_T), this);
  shortcut["newTab"]->setContext(Qt::ApplicationShortcut);
  connect(shortcut["newTab"], &QShortcut::activated, this, &MainWindow::on_actionNew_Tab_triggered);

  shortcut["closeTab"] = new QShortcut(QKeyCombination(Qt::CTRL, Qt::Key_D), this);
  shortcut["closeTab"]->setContext(Qt::ApplicationShortcut);
  connect(shortcut["closeTab"],
          &QShortcut::activated,
          this,
          &MainWindow::on_actionClose_Tab_triggered);
}

MainWindow::~MainWindow() {
  for (auto &i : shortcut)
    delete i.second;

  delete authorDialog;
  delete workspace;
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
    workspace->getCurrentModel()->readFromFile(fileName);

    //ui->actionFileOpen->setDisabled(true);
    ui->actionFileClose->setEnabled(true);
    ui->actionFileUpdate->setEnabled(true);
    ui->actionFileWrite->setEnabled(true);
  }
}

void MainWindow::on_actionFileWrite_triggered() {
  workspace->getCurrentModel()->writeToFile();
}

void MainWindow::on_actionFileUpdate_triggered() {
  workspace->getCurrentModel()->readFromFile();
}
void MainWindow::on_actionFileClose_triggered() {
  workspace->getCurrentModel()->reset();
  /*
  ui->actionFileOpen->setEnabled(true);
  ui->actionFileClose->setDisabled(true);
  ui->actionFileUpdate->setDisabled(true);
  ui->actionFileWrite->setDisabled(true);
  */
}

void MainWindow::on_actionNew_Tab_triggered() {
  if (!workspaceInitialized) {
    workspace->setLayout(ui->gridLayout_2);
    ui->gridLayout_2->addWidget(workspace);
    workspace->show();

    workspaceInitialized = true;
  }

  ui->label->setHidden(true);
  if (workspace->isHidden()) {
    workspace->setHidden(false);
  }

  workspace->addTab();
}

void MainWindow::on_actionClose_Tab_triggered() {
  if (!workspaceInitialized)
    return;

  auto *tabWidget = workspace->getTabWidget();

  tabWidget->removeTab(tabWidget->currentIndex());
  if (tabWidget->count() == 0) {
    workspace->setHidden(true);
    ui->label->setHidden(false);
  }
}
