#pragma once

#include <QObject>
#include <QTableView>

class GuidedTable : public QTableView {
  Q_OBJECT
public:
  GuidedTable(QWidget *parent = nullptr);
};
