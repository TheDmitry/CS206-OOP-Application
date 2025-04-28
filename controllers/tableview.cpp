#include "controllers/tableview.h"
#include "models/customtablemodel.h"
#include "ui_tableview.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QSizePolicy>
#include <QTableView>

#include "models/customsortfilterproxymodel.h"

TableView::TableView(QWidget *parent)
  : QWidget(parent)
  , model(new CustomTableModel(this))
  , proxyModel(new CustomSortFilterProxyModel(this))
  , shortcut{}
  , ui(new Ui::TableView) {
  ui->setupUi(this);
  ui->searchLine->setHidden(true);

  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyModel->setFilterKeyColumn(0);
  proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

  ui->tableView->setModel(proxyModel);

  connectSignals();

  initShortcuts();
}

TableView::~TableView() {
  for (auto &i : shortcut)
    delete i.second;

  delete proxyModel;
  delete model;
  delete ui;
}

void TableView::connectSignals() {
  connect(ui->tableView->selectionModel(),
          &QItemSelectionModel::currentColumnChanged,
          this,
          [this](QModelIndex const &current) {
            int column = current.column();

            if (current.column() < 0) {
              ui->searchLine->setHidden(true);
              column = 0;
            } else
              ui->searchLine->setHidden(false);
            proxyModel->setFilterKeyColumn(column);
          });
}

void TableView::initShortcuts() {
  shortcut["clear"] = new QShortcut(QKeyCombination(Qt::ALT, Qt::Key_R), this);
  shortcut["clear"]->setContext(Qt::WindowShortcut);
  connect(shortcut["clear"], &QShortcut::activated, this, &TableView::on_pushButton_clear_clicked);

  shortcut["removeRow"] = new QShortcut(Qt::Key_Delete, this);
  shortcut["removeRow"]->setContext(Qt::WindowShortcut);
  connect(shortcut["removeRow"],
          &QShortcut::activated,
          this,
          &TableView::on_pushButton_remove_clicked);

  shortcut["undo"] = new QShortcut(QKeyCombination(Qt::CTRL, Qt::Key_Z), this);
  shortcut["undo"]->setContext(Qt::WindowShortcut);
  connect(shortcut["undo"], &QShortcut::activated, this, &TableView::on_pushButton_undo_clicked);
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
  if (model->isEmpty())
    return;

  auto selected = ui->tableView->selectionModel()->currentIndex();
  if (!selected.isValid() || selected.column() < 0)
    return;

  QModelIndex proxyIndex = selected;
  QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);

  model->removeRow(sourceIndex.row());
}

void TableView::on_pushButton_undo_clicked() {
  model->rewind();
}

void TableView::on_searchLine_textChanged(const QString &arg1) {
  if (model->isEmpty())
    return;

  proxyModel->setFilterRegularExpression(arg1);
}
