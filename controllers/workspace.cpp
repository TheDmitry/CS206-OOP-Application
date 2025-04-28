#include "workspace.h"
#include "ui_workspace.h"

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
                        QString::fromStdString("Tab " + to_string(ui->tabWidget->count())));

  emit tabCreated(ui->tabWidget->count() - 1);
}

QTabWidget *Workspace::getTabWidget() {
  return ui->tabWidget;
};

TableView *Workspace::getCurrentWidget() {
  return static_cast<TableView *>(getTabWidget()->currentWidget());
}

QTableView *Workspace::getCurrentTableView() {
  return getCurrentWidget()->getTable();
};

CustomTableModel *Workspace::getCurrentModel() {
  return static_cast<CustomTableModel *>(getCurrentWidget()->getTableModel());
};

void Workspace::on_tabWidget_currentChanged(int index) {
  emit tabChanged(index);
}
