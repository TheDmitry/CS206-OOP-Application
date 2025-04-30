#include <QActionGroup>
#include <QFileDialog>
#include <QLibraryInfo>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , authorDialog(new AuthorDialog(this))
  , errorDialog(new ErrorDialog(this))
  , workspace(new Workspace(this))
  , workspaceInitialized{false}
  , settings{"TermPaperMGSU", "OOP-Term-Paper-Application"}
  , shortcut{}
  , ui(new Ui::MainWindow)
  , languageActionGroup(nullptr) {
  ui->setupUi(this);
  workspace->setHidden(true);

  connectSignals();
  initShortcuts();
  initTranslations();

  readSettings();
}

MainWindow::~MainWindow() {
  writeSettings();

  for (auto &i : shortcut)
    delete i.second;

  delete authorDialog;
  delete errorDialog;
  delete workspace;
  delete ui;
}

void MainWindow::initWorkspace() {
  workspace->setLayout(ui->gridLayout_2);
  ui->gridLayout_2->addWidget(workspace);
  workspace->show();

  workspaceInitialized = true;
}

void MainWindow::readSettings() {
  settings.beginGroup("Main");

  QString language = settings.value("language", "en_US").toString();
  for (auto &i : languageActionGroup->actions()) {
    QString actionLanguage = i->data().toString();

    if (language != actionLanguage) {
      i->setChecked(false);
      continue;
    }

    i->setChecked(true);
    switchLanguage(i);
  }

  settings.endGroup();
}

void MainWindow::writeSettings() {
  settings.beginGroup("Main");
  settings.setValue("language", getLanguage());
  settings.endGroup();
}

void MainWindow::initTranslations() {
  qApp->installTranslator(&appTranslator);
  qApp->installTranslator(&qtTranslator);
  qmPath = qApp->applicationDirPath() + "/translations";
  createLanguageMenu();
}

void MainWindow::createLanguageMenu() {
  languageActionGroup = new QActionGroup(this);

  connect(languageActionGroup, &QActionGroup::triggered, this, &MainWindow::switchLanguage);

  QDir dir(qmPath);

  QStringList fileNames = dir.entryList(QStringList("project_*.qm"));

  for (int i = 0; i < fileNames.size(); i++) {
    QString locale = fileNames[i];
    locale.remove(0, locale.indexOf('_') + 1);
    locale.truncate(locale.lastIndexOf('.'));

    QTranslator translator;
    if (!translator.load(fileNames[i], qmPath)) {
      QMessageBox::critical(this,
                            tr("Application"),
                            tr("The translation file could not be loaded!"),
                            QMessageBox::Ok);
      return;
    }

    QString language = translator.translate("MainWindow", "English");
    QAction *action = new QAction(tr("&%1 %2").arg(QString::number(i + 1), language), this);

    action->setCheckable(true);
    action->setData(locale);

    ui->menuLanguage->addAction(action);
    languageActionGroup->addAction(action);

    if (language == "English")
      action->setChecked(true);
  }
}

void MainWindow::switchLanguage(QAction *action) {
  QString locale = action->data().toString();

  if (!appTranslator.load("project_" + locale, qmPath)) {
    QMessageBox::critical(this,
                          tr("Application"),
                          tr("The translation file could not be loaded!"),
                          QMessageBox::Ok);
    return;
  }

  locale.chop(3);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
  if (!qtTranslator.load("qt_" + locale + ".qm",
                         QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
    QMessageBox::critical(this,
                          tr("Application"),
                          tr("The translation file could not be loaded!"),
                          QMessageBox::Ok);
    return;
  }
#else
  if (!qtTranslator.load("qt_" + locale + ".qm",
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
    QMessageBox::critical(this,
                          tr("Application"),
                          tr("The translation file could not be loaded!"),
                          QMessageBox::Ok);
    return;
  }
#endif

  ui->retranslateUi(this);
}

QString MainWindow::getLanguage() {
  for (auto &i : languageActionGroup->actions()) {
    if (i->isChecked())
      return i->data().toString();
  }

  return "en_US"; // default - English!
}

void MainWindow::connectSignals() {
  connect(workspace, &Workspace::tabChanged, this, &MainWindow::checkFileTabs);
  connect(workspace, &Workspace::tabCreated, this, &MainWindow::checkWorkspaceTabs);
  connect(workspace, &Workspace::tabClosed, this, [this]() {
    checkFileTabs();
    checkWorkspaceTabs();
  });
}

void MainWindow::initShortcuts() {
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

void MainWindow::checkFileTabs() {
  bool hasData = false;

  if (workspaceInitialized && workspace->getTabWidget()->count() > 0)
    hasData = !workspace->getCurrentModel()->isEmpty();

  ui->actionFileOpen->setEnabled(!hasData);
  ui->actionFileClose->setEnabled(hasData);
  ui->actionFileUpdate->setEnabled(hasData);
  ui->actionFileWrite->setEnabled(hasData);
}

void MainWindow::checkWorkspaceTabs() {
  bool hasTabs = workspaceInitialized && workspace->getTabWidget()->count() > 0;

  ui->actionClose_Tab->setEnabled(hasTabs);
  ui->actionClose_All_Tabs->setEnabled(hasTabs);
}

void MainWindow::on_actionProgramAuthor_triggered() {
  authorDialog->exec();
}

void MainWindow::on_actionFileOpen_triggered() {
  string fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Read file"),
                                                 QDir::currentPath(),
                                                 "Db Files (.db);;All Files (.*)")
                      .toStdString();
  if (!fileName.empty()) {
    if (!workspaceInitialized || workspace->getTabWidget()->count() == 0)
      on_actionNew_Tab_triggered();

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
  if (!workspaceInitialized)
    initWorkspace();

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
    tabWidget->clear();
    ui->label->setHidden(false);
    workspace->setHidden(true);
    return;
  }

  auto *t = tabWidget->widget(tabWidget->currentIndex());
  tabWidget->removeTab(tabWidget->currentIndex());
  delete t;
}

void MainWindow::on_actionClose_All_Tabs_triggered() {
  if (!workspaceInitialized)
    return;

  workspace->getTabWidget()->clear();
  ui->label->setHidden(false);
  workspace->setHidden(true);
}
