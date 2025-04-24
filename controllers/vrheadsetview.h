#ifndef VRHEADSETVIEW_H
#define VRHEADSETVIEW_H

#include <QWidget>

#include "models/vrheadsettablemodel.h"

namespace Ui {
class VRHeadsetView;
}

class VRHeadsetView : public QWidget {
  Q_OBJECT

public:
  explicit VRHeadsetView(QWidget *parent = nullptr);
  ~VRHeadsetView();

private:
  VRHeadsetTableModel *headsetTableModel;
  Ui::VRHeadsetView *ui;
};

#endif // VRHEADSETVIEW_H
