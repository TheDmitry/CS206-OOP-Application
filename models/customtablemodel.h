#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "external/dbfile.h"
#include "items/abstractitem.h"

class CustomTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit CustomTableModel(QObject *parent = nullptr);
  ~CustomTableModel();

  // Header:
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  bool setHeaderData(int section,
                     Qt::Orientation orientation,
                     const QVariant &value,
                     int role = Qt::EditRole) override;

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  void reset();
  void clear();
  // string const -> string const &
  void setFileName(string const fileName);

  void readFromFile(string const fileName);
  void readFromFile();

  void writeToFile(string const fileName);
  void writeToFile();

private:
  vector<unique_ptr<AbstractItem>> items;

  // DbFile workflow
  DbFile db;
};

#endif // CUSTOMTABLEMODEL_H
