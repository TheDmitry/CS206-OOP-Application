#include "controllers/tableview.h"
#include "models/customtablemodel.h"
#include "ui_tableview.h"

#include <QHeaderView>
#include <QSizePolicy>
#include <QSortFilterProxyModel>
#include <QTableView>

TableView::TableView(QWidget *parent)
  : QWidget(parent)
  , model(new CustomTableModel(this))
  , shortcut{}
  , ui(new Ui::TableView) {
  ui->setupUi(this);

  ui->tableView->setModel(model);
  ui->tableView->setSortingEnabled(true);

  initShortcuts();
}

TableView::~TableView() {
  for (auto &i : shortcut)
    delete i.second;

  delete proxyModel;
  delete model;
  delete ui;
}

void TableView::initShortcuts() {
  cout << "Shortcuts for TableView initialized" << endl;

  shortcut["clear"] = new QShortcut(QKeyCombination(Qt::ALT, Qt::Key_R), this);
  shortcut["clear"]->setContext(Qt::WindowShortcut);
  connect(shortcut["clear"], &QShortcut::activated, this, &TableView::on_pushButton_clear_clicked);

  shortcut["removeRow"] = new QShortcut(QKeyCombination(Qt::ALT, Qt::Key_D), this);
  shortcut["removeRow"]->setContext(Qt::WindowShortcut);
  connect(shortcut["removeRow"],
          &QShortcut::activated,
          this,
          &TableView::on_pushButton_remove_clicked);
}

void TableView::on_pushButton_clear_clicked() {
  model->reset();
}

QTableView *TableView::getTable() {
  return ui->tableView;
}

CustomTableModel *TableView::getTableModel() {
  return model;
}

void TableView::on_pushButton_new_clicked() {
  model->addEmptyRow();
}

void TableView::on_pushButton_remove_clicked() {
  auto selected = ui->tableView->selectionModel()->selectedIndexes();

  model->removeRow(selected.first().row());
}

void TableView::on_pushButton_undo_clicked() {
  model->rewind();
}
