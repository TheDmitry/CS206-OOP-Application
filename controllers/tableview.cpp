#include "tableview.h"
#include "models/customtablemodel.h"
#include "ui_tableview.h"

TableView::TableView(QWidget *parent)
  : QWidget(parent)
  , customTableModel{new CustomTableModel(this)}
  , ui(new Ui::TableView) {
  ui->setupUi(this);
  ui->tableView->setModel(customTableModel);
}

TableView::~TableView() {
  delete customTableModel;
  delete ui;
}

void TableView::on_pushButton_clear_clicked() {
  customTableModel->reset();
}

QTableView *TableView::getTable() {
  return ui->tableView;
}

CustomTableModel *TableView::getTableModel() {
  return static_cast<CustomTableModel *>(getTable()->model());
}

void TableView::on_pushButton_new_clicked() {
  getTableModel()->addEmptyRow();
}
