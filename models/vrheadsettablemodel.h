#ifndef VRHEADSETTABLEMODEL_H
#define VRHEADSETTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "external/dbfile.h"
#include "items/abstractitem.h"

class VRHeadsetTableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit VRHeadsetTableModel(QObject *parent = nullptr);

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

  void setFileName(string const fileName);

  void readFromFile(string const fileName);
  void readFromFile();

  void writeToFile(string const fileName);
  void writeToFile();

private:
  vector<unique_ptr<AbstractItem>> headsets;

  // DbFile workflow
  DbFile db;

  int columnsNum;
};

#endif // VRHEADSETTABLEMODEL_H
