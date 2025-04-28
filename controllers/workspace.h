#ifndef WORKSPACE_H
#define WORKSPACE_H

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

signals:
  void tabChanged(int index);
  void tabCreated(int index);
  void tabClosed(int index);

private slots:
  void on_tabWidget_currentChanged(int index);

private:
  Ui::Workspace *ui;

  void changeEvent(QEvent *e) override;
};

#endif // WORKSPACE_H
