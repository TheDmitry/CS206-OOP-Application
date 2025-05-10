#include "simpchart.h"
#include "ui_simpchart.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QPointF>
#include <QString>

using namespace std;

SimpChart::SimpChart(std::shared_ptr<AbstractProvider> const &provider,
                     std::vector<std::shared_ptr<AbstractItem>> const &items,
                     std::string const &title,
                     QWidget *parent)
  : QWidget(parent)
  , provider(provider)
  , items(items)
  , ui(new Ui::SimpChart) {
  ui->setupUi(this);

  ui->chart->setChart(new QChart());
  ui->chart->setRenderHint(QPainter::Antialiasing);

  ui->chart->chart()->setAnimationOptions(QChart::SeriesAnimations);
  ui->chart->chart()->setTitle(QString::fromStdString(title));

  setWindowTitle(QString::fromStdString(title));

  for (auto const &i : provider->getChartFields())
    ui->chart_select->addItem(QString::fromStdString(i));

  if (ui->chart_select->count() > 0)
    plotGraph(ui->chart_select->currentText().toStdString());

  connectSignals();
}

void SimpChart::connectSignals() {
  connect(ui->chart_select, &QComboBox::currentIndexChanged, this, [this]() {
    qDebug() << "test";
    ui->chart->chart()->removeAllSeries();
    plotGraph(ui->chart_select->currentText().toStdString());
  });
}

void SimpChart::plotGraph(std::string const &field) {
  QBarSeries *series = new QBarSeries();
  auto const &chart = ui->chart->chart();

  for (auto const &i : items) {
    QBarSet *set = new QBarSet(
      QString::fromStdString(provider->get(i, provider->getChartHeaderField())));
    *set << stod(provider->get(i, field));
    series->append(set);
  }

  chart->addSeries(series);
  chart->createDefaultAxes();
}

SimpChart::~SimpChart() {
  delete ui;
}
