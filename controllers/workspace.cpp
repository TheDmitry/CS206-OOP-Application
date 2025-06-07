#include "workspace.h"
#include "ui_workspace.h"

#include "controllers/mainwindow.h"

using namespace std;

Workspace::Workspace(QWidget *parent)
    : QWidget(parent), contextMenu(new QMenu(tr("Workspace menu"), this)),
      ui(new Ui::Workspace) {
  ui->setupUi(this);

  ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

  initContextActions();
  initContextMenu();
}

Workspace::~Workspace() {
  for (auto &i : contextActions)
    delete i.second;

  delete ui;
}

void Workspace::initContextActions() {
  auto *prt = static_cast<MainWindow *>(parent());

  contextActions["OpenFile"] = new QAction(tr("Open File"), this);
  connect(contextActions["OpenFile"], &QAction::triggered, prt,
          [prt]() { prt->on_actionFileOpen_triggered(); });

  contextActions["CloseTab"] = new QAction(tr("Close Tab"), this);
  connect(contextActions["CloseTab"], &QAction::triggered, this,
          &Workspace::closeTab);

  contextActions["CloseFile"] = new QAction(tr("Close File"), this);
  connect(contextActions["CloseFile"], &QAction::triggered, prt,
          [prt]() { prt->on_actionFileClose_triggered(); });

  contextActions["WriteFile"] = new QAction(tr("Write to File"), this);
  connect(contextActions["WriteFile"], &QAction::triggered, prt,
          [prt]() { prt->on_actionFileWrite_triggered(); });

  contextActionsOrder = {"OpenFile", "WriteFile", "CloseFile", "CloseTab"};
}

void Workspace::initContextMenu() {
  for (auto const &i : contextActionsOrder) {
    contextMenu->addAction(contextActions[i]);
  }

  connect(ui->tabWidget, &QTabWidget::customContextMenuRequested, this,
          [this]() {
            for (auto &i : contextActions)
              i.second->setEnabled(true);

            if (!getCurrentModel()->isEmpty())
              contextActions["OpenFile"]->setEnabled(false);
            else {
              contextActions["WriteFile"]->setEnabled(false);
              contextActions["CloseFile"]->setEnabled(false);
            }

            contextMenu->popup(QCursor::pos());
          });
}

void Workspace::retranslateContextActions() {
  contextMenu->setTitle(tr(contextMenu->title().toStdString().c_str()));
  for (auto &i : contextActions)
    i.second->setText(tr(i.second->text().toStdString().c_str()));
}

void Workspace::addTab() {
  int widget = ui->tabWidget->addTab(
      new TableView,
      QString::fromStdString("Tab " + to_string(ui->tabWidget->count())));
  connect(static_cast<TableView *>(ui->tabWidget->widget(widget)),
          &TableView::dropHappened, this, &Workspace::dropHappened);

  emit tabCreated(ui->tabWidget->count() - 1);
}

void Workspace::closeTab() {
  getTabWidget()->removeTab(getTabWidget()->currentIndex());

  emit tabClosed();
}

void Workspace::closeAllTabs() {
  while (getTabWidget()->count() > 0)
    getTabWidget()->removeTab(0);

  emit tabClosed();
}

QTabWidget *Workspace::getTabWidget() { return ui->tabWidget; };

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

void Workspace::changeEvent(QEvent *e) {
  if (e->type() == QEvent::LanguageChange) {
    ui->retranslateUi(this);
    retranslateContextActions();
  }

  QWidget::changeEvent(e);
}
