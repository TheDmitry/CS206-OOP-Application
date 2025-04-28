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
  , proxyModel(new QSortFilterProxyModel(this))
  , shortcut{}
  , ui(new Ui::TableView) {
  ui->setupUi(this);

  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

  ui->tableView->setModel(proxyModel);

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
  if (selected.isEmpty())
    return;

  QModelIndex proxyIndex = selected.first();
  QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);

  model->removeRow(sourceIndex.row());
}

void TableView::on_pushButton_undo_clicked() {
  model->rewind();
}

void TableView::on_lineEdit_textChanged(const QString &arg1) {
  proxyModel->setFilterFixedString(arg1);
  cout << arg1.toStdString() << endl;
}
