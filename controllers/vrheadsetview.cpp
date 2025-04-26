#include "vrheadsetview.h"
#include "models/customtablemodel.h"
#include "ui_vrheadsetview.h"

VRHeadsetView::VRHeadsetView(QWidget *parent)
  : QWidget(parent)
  , customTableModel{new CustomTableModel(this)}
  , ui(new Ui::VRHeadsetView) {
  ui->setupUi(this);
  ui->tableView->setModel(customTableModel);
}

VRHeadsetView::~VRHeadsetView() {
  delete customTableModel;
  delete ui;
}

void VRHeadsetView::on_pushButton_clear_clicked() {
  customTableModel->reset();
}

QTableView *VRHeadsetView::getTable() {
  return ui->tableView;
}

CustomTableModel *VRHeadsetView::getTableModel() {
  return static_cast<CustomTableModel *>(getTable()->model());
}
