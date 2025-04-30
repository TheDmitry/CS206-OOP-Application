#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
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

  /*!
   * \brief switchLanguage
   * \param QAction* action
   * Slot, fired when changing of current language is performed
   */
  void switchLanguage(QAction *action);

private:
  AuthorDialog *authorDialog;
  ErrorDialog *errorDialog;
  Workspace *workspace;
  bool workspaceInitialized;
  QSettings settings;

  std::map<std::string, QShortcut *> shortcut;

  Ui::MainWindow *ui;

  QTranslator appTranslator;
  QTranslator qtTranslator;

  QActionGroup *languageActionGroup;
  QString qmPath;

  /*!
   * \brief checkFileTabs
   * Redraw available file options depending on state of workspace 
   */
  void checkFileTabs(); // Redraw available file options

  /*!
   * \brief checkWorkspaceTabs
   * Redraw available tab options depending on state of workspace 
   */
  void checkWorkspaceTabs(); // Redraw available tab options

  /*!
   * \brief createLanguageMenu
   * Create language menu depending on state of ./translation directory
   */
  void createLanguageMenu(); // Language menu, currently for en_US and ru_RU

  /*!
   * \brief getLanguage
   * \return QString with name of currently selected language
   */
  QString getLanguage(); // Returns current language or en_US if none is set

  /*!
   * \brief connectSignals
   * Initialize and connect signals related to checkFileTabs() and checkWorspaceTabs()
   */
  void connectSignals();

  /*!
   * \brief initShortcuts
   * Initialize and connect application-based shortcuts
   */
  void initShortcuts();

  /*!
   * \brief initTranslations
   * Setup translations depending on state of ./translation directory 
   */
  void initTranslations(); // Translations: ru_RU and en_US

  /*!
   * \brief initWorkspace
   * Initialize workspace UI
   * Should be called when you need to setup workspace for the first time
   * 
   */
  void initWorkspace();

  /*!
   * \brief readSettings
   * Load all settings and change whatever is needed depending on them
   */
  void readSettings();

  /*!
   * \brief writeSettings
   * Write all setting-dependent stuff
   * Should be called in destructor of MainWindow
   */
  void writeSettings();
};
#endif // MAINWINDOW_H
