#pragma once

#include <QAbstractTableModel>
#include <QMimeData>
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

  /*!
   * \brief CustomTableModel constructor
   */
  explicit CustomTableModel(QObject *parent = nullptr);

  /*!
   * \brief CustomTableModel destructor
   */
  ~CustomTableModel();

  /*!
   * \brief Returns header data from exact provider
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /*!
   * \brief Sets header data by exact provider
   */
  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role = Qt::EditRole) override;

  /*!
   * \brief Mime types specifications for Drag&Drop functionality
   */
  QStringList mimeTypes() const override;

  QMimeData *mimeData(QModelIndexList const &indexes) const override;

  bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row,
                    int column, QModelIndex const &parent) override;

  Qt::DropActions supportedDropActions() const override;

  // Rows
  int rowCount(QModelIndex const &parent = QModelIndex()) const override;

  int columnCount(QModelIndex const &parent = QModelIndex()) const override;

  QVariant data(QModelIndex const &index,
                int role = Qt::DisplayRole) const override;

  bool setData(QModelIndex const &index, QVariant const &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(QModelIndex const &index) const override;

  bool insertRows(int row, int count, QModelIndex const &parent) override;
  bool removeRows(int row, int count, const QModelIndex &parent) override;
  bool insertRow(int row, QModelIndex const &parent);
  bool removeRow(int row, QModelIndex const &parent);

  // DbFile realted
  void reset();
  void clear();

  void setFileName(std::string const &fileName);

  void readFromFile(std::string const &fileName);
  void readFromFile();

  void writeToFile(std::string const &fileName);
  void writeToFile();

  void makeEmpty(std::string const &providerName);

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
  void emptyAssigned(std::string const &providerName);
  void dropHappened(bool result);

private:
  ContainerType items;
  Undoer<ContainerType> undoer;

  // DbFile workflow
  DbFile db;
};
