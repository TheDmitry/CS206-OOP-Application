#pragma once

#include "items/abstractitem.h"
#include "providers/abstractprovider.h"

#include <QChart>
#include <QWidget>
#include <memory>
#include <vector>

namespace Ui {
class SimpChart;
}

class SimpChart : public QWidget {
  Q_OBJECT

public:
  explicit SimpChart(std::shared_ptr<AbstractProvider> const &provider,
                     std::vector<std::shared_ptr<AbstractItem>> const &items,
                     std::string const &title = tr("Simple Chart").toStdString(),
                     QWidget *parent = nullptr);
  ~SimpChart();

private:
  void connectSignals();
  void plotGraph(std::string const &field);

  std::shared_ptr<AbstractProvider> const &provider;
  std::vector<std::shared_ptr<AbstractItem>> const &items;

  Ui::SimpChart *ui;
};
