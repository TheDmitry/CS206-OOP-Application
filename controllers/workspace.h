#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QMenu>
#include <QTabWidget>
#include <QTableView>
#include <QWidget>

#include "controllers/tableview.h"

#include "models/customtablemodel.h"

namespace Ui {
class Workspace;
}

class Workspace : public QWidget {
  Q_OBJECT

public:
  explicit Workspace(QWidget *parent = nullptr);
  ~Workspace();

  QTabWidget *getTabWidget();

  TableView *getCurrentWidget();
  QTableView *getCurrentTableView();
  CustomTableModel *getCurrentModel();

  void addTab();
  void closeTab();
  void closeAllTabs();

signals:
  void tabChanged(int index);
  void tabCreated(int index);
  void tabClosed();

private slots:
  void on_tabWidget_currentChanged(int index);

private:
  QMenu *contextMenu;
  std::map<std::string, QAction *> contextActions;
  std::vector<std::string> contextActionsOrder;

  Ui::Workspace *ui;

  void initContextActions();
  void initContextMenu();
  void retranslateContextActions();

  void changeEvent(QEvent *e) override;
};

#endif // WORKSPACE_H
