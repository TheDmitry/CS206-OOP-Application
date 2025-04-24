#include "vrheadsetview.h"
#include "models/vrheadsettablemodel.h"
#include "ui_vrheadsetview.h"

VRHeadsetView::VRHeadsetView(QWidget *parent)
  : QWidget(parent)
  , headsetTableModel{new VRHeadsetTableModel(this)}
  , ui(new Ui::VRHeadsetView) {
  ui->setupUi(this);
  ui->tableView->setModel(headsetTableModel);
}

VRHeadsetView::~VRHeadsetView() {
  delete headsetTableModel;
  delete ui;
}
