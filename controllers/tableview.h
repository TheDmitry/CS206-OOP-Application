#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QWidget>

#include "models/customtablemodel.h"

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

private:
  CustomTableModel *customTableModel;
  Ui::TableView *ui;
};

#endif // TABLEVIEW_H
