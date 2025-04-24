#include "workspace.h"
#include <QString>
#include "ui_workspace.h"
#include <string>

#include "controllers/vrheadsetview.h"

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
  auto *vrheadview = new VRHeadsetView;
  VRHeadsetViews.push_back(vrheadview);

  vrheadview->setLayout(ui->gridLayout);
  ui->gridLayout->addWidget(vrheadview);

  ui->tabWidget->addTab(vrheadview,
                        QString::fromStdString("Tab " + std::to_string(VRHeadsetViews.size())));
  vrheadview->show();
}
