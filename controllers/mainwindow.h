#pragma once

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

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  // These slots are opened cuz they are used in tableview context actions

  /*!
   * \brief File -> Open
   */
  void on_actionFileOpen_triggered();

  /*!
   * \brief File -> Write
   */
  void on_actionFileWrite_triggered();

  /*!
   * \brief File -> Update
   */
  void on_actionFileUpdate_triggered();

  /*!
   * \brief File -> Close
   */
  void on_actionFileClose_triggered();

  /*!
   * \brief Workspace -> Close Tab
   */
  void on_actionClose_Tab_triggered();

private slots:
  /*!
   * \brief Program -> Author
   */
  void on_actionProgramAuthor_triggered();

  /*!
   * \brief switchLanguage
   * \param QAction* action
   * Slot, fired when changing of current language is performed
   */
  void switchLanguage(QAction *action);

  /*!
   * \brief Workspace -> Close all
   */
  void on_actionClose_All_Tabs_triggered();

  /*!
   * \brief Workspace -> New tab
   */
  void on_actionNew_Tab_triggered();

  /*!
   * \brief File -> Print
   */
  void on_actionFilePrint_triggered();

private:
  /*!
   * \brief Dialog which is shown whenever user clicks Program -> Author
   */
  AuthorDialog *authorDialog;

  /*!
   * \brief Dialog which is shown whenever any handled error happens in application
   */
  ErrorDialog *errorDialog;

  /*!
   * \brief Workspace is main part of this program. It's used to manipulate, read and write any data
   */
  Workspace *workspace;

  /*!
   * \brief Represents current state of workspace 
   */
  bool workspaceInitialized;

  /*!
   * \brief Settings related to stuff in MainWindow
   */
  QSettings settings;

  /*!
   * \brief shortcutName -> QShortcut association which is used in initShortcuts()
   */
  std::map<std::string, QShortcut *> shortcut;

  /*!
   * \brief Qt related UI pointer
   */
  Ui::MainWindow *ui;

  /*!
   * \brief Related to translation
   */
  QTranslator appTranslator;

  /*!
   * \brief Related to translation
   */
  QTranslator qtTranslator;

  /*!
   * \brief ActionGroup which stores any checkable object for language switching
   */
  QActionGroup *languageActionGroup;

  /*!
   * \brief Path to translation directory, which is cwd/translations by default
   */
  QString qmPath;

  /*!
   * \brief Redraw available file options depending on state of workspace 
   */
  void checkFileTabs();

  /*!
   * \brief Redraw available tab options depending on state of workspace 
   */
  void checkWorkspaceTabs();

  /*!
   * \brief Create language menu depending on state of ./translation directory
   */
  void createLanguageMenu();

  /*!
   * \brief Can be used to get currently selected language
   * \return QString
   */
  QString getLanguage();

  /*!
   * \brief Initialize and connect signals related to checkFileTabs() and checkWorkspaceTabs()
   */
  void connectSignals();

  /*!
   * \brief Initialize and connect application-based shortcuts
   */
  void initShortcuts();

  /*!
   * \brief Setup translations depending on state of ./translation directory 
   */
  void initTranslations();

  /*!
   * \brief Initialize workspace UI
   * 
   * Important: Should be called when you need to setup workspace for the first time
   */
  void initWorkspace();

  /*!
   * \brief Load all settings and change whatever is needed depending on them
   */
  void readSettings();

  /*!
   * \brief Write all setting-dependent stuff
   * 
   * Important: Should be called in destructor of MainWindow
   */
  void writeSettings();
};
