#include "workspace.h"
#include <QString>
#include "ui_workspace.h"
#include <string>

#include "controllers/tableview.h"

using namespace std;

Workspace::Workspace(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Workspace) {
  ui->setupUi(this);
}

Workspace::~Workspace() {
  delete ui;
}

void Workspace::addTab() {
  ui->tabWidget->addTab(new TableView,
                        QString::fromStdString("Tab " + std::to_string(ui->tabWidget->count())));

  emit tabCreated(ui->tabWidget->count() - 1);
}

QTabWidget *Workspace::getTabWidget() {
  return ui->tabWidget;
};

QTableView *Workspace::getCurrentTableView() {
  return static_cast<TableView *>(ui->tabWidget->currentWidget())->getTable();
};

CustomTableModel *Workspace::getCurrentModel() {
  return static_cast<CustomTableModel *>(getCurrentTableView()->model());
};

void Workspace::on_tabWidget_currentChanged(int index) {
  emit tabChanged(index);
}
