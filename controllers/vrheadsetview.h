#ifndef VRHEADSETVIEW_H
#define VRHEADSETVIEW_H

#include <QTableView>
#include <QWidget>

#include "models/customtablemodel.h"

namespace Ui {
class VRHeadsetView;
}

class VRHeadsetView : public QWidget {
  Q_OBJECT

public:
  explicit VRHeadsetView(QWidget *parent = nullptr);
  ~VRHeadsetView();
  QTableView *getTable();

  CustomTableModel *getTableModel();

private slots:
  void on_pushButton_clear_clicked();

private:
  CustomTableModel *customTableModel;
  Ui::VRHeadsetView *ui;
};

#endif // VRHEADSETVIEW_H
