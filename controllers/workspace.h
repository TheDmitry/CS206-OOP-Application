#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include "controllers/vrheadsetview.h"
#include <vector>

namespace Ui {
class Workspace;
}

class Workspace : public QWidget {
  Q_OBJECT

public:
  explicit Workspace(QWidget *parent = nullptr);
  ~Workspace();

  void addTab();

private:
  vector<VRHeadsetView *> VRHeadsetViews;
  Ui::Workspace *ui;
};

#endif // WORKSPACE_H
