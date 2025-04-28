#include "customsortfilterproxymodel.h"
#include "models/customtablemodel.h"

using namespace std;

CustomSortFilterProxyModel::CustomSortFilterProxyModel(QObject *parent)
  : QSortFilterProxyModel{parent} {}

bool CustomSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
  auto *model = static_cast<CustomTableModel *>(sourceModel());
  auto &provider = model->getDb().getProvider();

  if (!provider)
    return QSortFilterProxyModel::lessThan(left, right);

  string fieldName = model->headerData(left.column(), Qt::Horizontal).toString().toStdString();

  auto &a = model->getItem(left.row());
  auto &b = model->getItem(right.row());

  return provider->compare(a, b, fieldName) < 0; // if compare returns < 0 it means a < b
}
