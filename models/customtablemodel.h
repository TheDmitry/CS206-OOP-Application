#pragma once

#include <QAbstractTableModel>
#include <vector>

#include "external/dbfile.h"
#include "external/undoer.h"
#include "items/abstractitem.h"

#define UNDO_SIZE 50;

class CustomTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  using ItemType = std::shared_ptr<AbstractItem>;
  using ContainerType = std::vector<ItemType>;

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
  int rowCount(QModelIndex const &parent = QModelIndex()) const override;
  int columnCount(QModelIndex const &parent = QModelIndex()) const override;

  QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(QModelIndex const &index, QVariant const &value, int role = Qt::EditRole) override;

  Qt::ItemFlags flags(QModelIndex const &index) const override;

  void reset();
  void clear();

  void setFileName(std::string const &fileName);

  void readFromFile(std::string const &fileName);
  void readFromFile();

  void writeToFile(std::string const &fileName);
  void writeToFile();

  void addEmptyRow();
  void removeRow(size_t row);
  ItemType &getItem(size_t row);
  ContainerType &getItems();
  bool isEmpty();

  /* Undo: no redo */
  void rewind();

  DbFile const &getDb() const;

signals:
  void fileLoaded(std::string const &fileName);
  void dbClear();
  void dbReset();

private:
  ContainerType items;
  Undoer<ContainerType> undoer;

  // DbFile workflow
  DbFile db;
};
