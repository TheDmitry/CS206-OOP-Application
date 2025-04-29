#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

#include "controllers/authordialog.h"
#include "controllers/errordialog.h"

#include "controllers/workspace.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private slots:
  void on_actionProgramAuthor_triggered();

  void on_actionFileOpen_triggered();

  void on_actionFileWrite_triggered();

  void on_actionFileUpdate_triggered();

  void on_actionFileClose_triggered();

  void on_actionNew_Tab_triggered();

  void on_actionClose_Tab_triggered();

  void switchLanguage(QAction *action);

private:
  AuthorDialog *authorDialog;
  ErrorDialog *errorDialog;
  Workspace *workspace;
  bool workspaceInitialized;

  std::map<std::string, QShortcut *> shortcut;

  Ui::MainWindow *ui;

  QTranslator appTranslator;
  QTranslator qtTranslator;

  QActionGroup *languageActionGroup;
  QString qmPath;

  void checkFileTabs();
  void checkWorkspaceTabs();
  void createLanguageMenu();

  void connectSignals();
  void initShortcuts();
  void initTranslations();

  void readSettings();
  void writeSettings();
};
#endif // MAINWINDOW_H
