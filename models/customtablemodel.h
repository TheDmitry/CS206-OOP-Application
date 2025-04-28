#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "external/dbfile.h"
#include "external/undoer.h"
#include "items/abstractitem.h"

#define UNDO_SIZE 50;

using namespace std;

class CustomTableModel : public QAbstractTableModel {
  Q_OBJECT
  using ItemType = shared_ptr<AbstractItem>;
  using Container = vector<ItemType>;

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

  void setFileName(string const &fileName);

  void readFromFile(string const &fileName);
  void readFromFile();

  void writeToFile(string const &fileName);
  void writeToFile();

  void addEmptyRow();
  void removeRow(size_t row);
  ItemType &getItem(size_t row);

  bool isEmpty();

  /* Undo: no redo */
  void rewind();

  DbFile const &getDb() const;

private:
  Container items;
  Undoer<Container> undoer;

  // DbFile workflow
  DbFile db;
};

#endif // CUSTOMTABLEMODEL_H
