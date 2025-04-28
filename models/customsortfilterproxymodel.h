#ifndef CUSTOMSORTFILTERPROXYMODEL_H
#define CUSTOMSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class CustomSortFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
public:
  explicit CustomSortFilterProxyModel(QObject *parent = nullptr);

  // QSortFilterProxyModel interface
protected:
  bool lessThan(QModelIndex const &left, QModelIndex const &right) const;
};

#endif // CUSTOMSORTFILTERPROXYMODEL_H
