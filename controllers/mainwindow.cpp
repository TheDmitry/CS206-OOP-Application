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

#include "external/dberror.h"
#include "external/parseerror.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , authorDialog(new AuthorDialog(this))
  , errorDialog(new ErrorDialog(this))
  , workspace(new Workspace())
  , workspaceInitialized(false)
  , shortcut{}
  , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  workspace->setHidden(true);

  connectTabSignals();
  initShortcuts();
}

MainWindow::~MainWindow() {
  for (auto &i : shortcut)
    delete i.second;

  delete authorDialog;
  delete errorDialog;
  delete workspace;
  delete ui;
}

void MainWindow::initShortcuts() {
  cout << "Shortcuts for MainWindow initialized!" << endl;

  shortcut["newTab"] = new QShortcut(QKeyCombination(Qt::CTRL, Qt::Key_W), this);
  shortcut["newTab"]->setContext(Qt::ApplicationShortcut);
  connect(shortcut["newTab"], &QShortcut::activated, this, &MainWindow::on_actionNew_Tab_triggered);

  shortcut["closeTab"] = new QShortcut(QKeyCombination(Qt::CTRL, Qt::Key_D), this);
  shortcut["closeTab"]->setContext(Qt::ApplicationShortcut);
  connect(shortcut["closeTab"],
          &QShortcut::activated,
          this,
          &MainWindow::on_actionClose_Tab_triggered);
}

void MainWindow::connectTabSignals() {
  connect(workspace, &Workspace::tabChanged, this, &MainWindow::checkFileTabs);
  connect(workspace, &Workspace::tabCreated, this, &MainWindow::checkWorkspaceTabs);
  connect(workspace, &Workspace::tabClosed, this, [this]() {
    checkFileTabs();
    checkWorkspaceTabs();
  });
}

void MainWindow::checkFileTabs() {
  bool hasData = false;

  if (workspaceInitialized && workspace->getTabWidget()->count() > 0)
    hasData = !workspace->getCurrentModel()->isEmpty();
  cout << "Call -> checkFileTabs() -> hasData(" << hasData << ")" << endl;

  ui->actionFileOpen->setEnabled(!hasData);
  ui->actionFileClose->setEnabled(hasData);
  ui->actionFileUpdate->setEnabled(hasData);
  ui->actionFileWrite->setEnabled(hasData);
}

void MainWindow::checkWorkspaceTabs() {
  bool hasTabs = workspaceInitialized && workspace->getTabWidget()->count() > 0;

  cout << "Call -> checkWorkspaceTabs() -> hasTabs(" << hasTabs << ")" << endl;

  ui->actionClose_Tab->setEnabled(hasTabs);
  ui->actionClose_All_Tabs->setEnabled(hasTabs);
}

void MainWindow::on_actionProgramAuthor_triggered() {
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
    try {
      workspace->getCurrentModel()->readFromFile(fileName);
    } catch (ParseError const &e) {
      errorDialog->callWithParseError(e);
      return;
    } catch (DbError const &e) {
      errorDialog->callWithDbError(e);
      return;
    }

    checkFileTabs();
  }
}

void MainWindow::on_actionFileWrite_triggered() {
  try {
    workspace->getCurrentModel()->writeToFile();
  } catch (ParseError const &e) {
    errorDialog->callWithParseError(e);
    return;
  } catch (DbError const &e) {
    errorDialog->callWithDbError(e);
    return;
  }
}

void MainWindow::on_actionFileUpdate_triggered() {
  try {
    workspace->getCurrentModel()->readFromFile();
  } catch (ParseError const &e) {
    errorDialog->callWithParseError(e);
    return;
  } catch (DbError const &e) {
    errorDialog->callWithDbError(e);
    return;
  }
}
void MainWindow::on_actionFileClose_triggered() {
  try {
    workspace->getCurrentModel()->reset();
  } catch (ParseError const &e) {
    errorDialog->callWithParseError(e);
    return;
  } catch (DbError const &e) {
    errorDialog->callWithDbError(e);
    return;
  }

  checkFileTabs();
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

  if ((tabWidget->count() - 1) <= 0) {
    ui->label->setHidden(false);
    workspace->setHidden(true);
  }

  auto *t = tabWidget->widget(tabWidget->currentIndex());
  tabWidget->removeTab(tabWidget->currentIndex());
  delete t;
}
