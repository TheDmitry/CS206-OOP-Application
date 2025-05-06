#pragma once

#include <QAction>
#include <QMenu>
#include <QShortcut>
#include <QTableView>
#include <QWidget>

#include "models/customsortfilterproxymodel.h"
#include "models/customtablemodel.h"

#include "delegates/highlightdelegate.h"

namespace Ui {
class TableView;
}

class TableView : public QWidget {
  Q_OBJECT

public:
  explicit TableView(QWidget *parent = nullptr);
  ~TableView();
  QTableView *getTable();

  CustomTableModel *getTableModel();

private slots:
  void on_pushButton_clear_clicked();

  void on_pushButton_new_clicked();

  void on_pushButton_remove_clicked();

  void on_pushButton_undo_clicked();

  void on_searchLine_textChanged(const QString &arg1);

  void on_pushButton_graph_clicked();

private:
  CustomTableModel *model;
  CustomSortFilterProxyModel *proxyModel;
  std::map<std::string, QShortcut *> shortcut;

  HighlightColumnDelegate *highlightDelegate;

  Ui::TableView *ui;

  void initShortcuts();
  void connectSignals();

  void changeEvent(QEvent *e) override;
};
