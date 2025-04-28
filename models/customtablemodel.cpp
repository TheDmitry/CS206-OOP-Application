#include "models/customtablemodel.h"

using namespace std;

CustomTableModel::CustomTableModel(QObject *parent)
  : QAbstractTableModel(parent)
  , items{} {}

CustomTableModel::~CustomTableModel() {
  items.clear();
  items.shrink_to_fit();

  db.reset();
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && !items.empty()) {
    return QString::fromStdString(db.getSchemeField(section));
  }

  if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
    return QString::number(section + 1);
  }

  return QVariant();
}

bool CustomTableModel::setHeaderData(int section,
                                     Qt::Orientation orientation,
                                     const QVariant &value,
                                     int role) {
  if (value != headerData(section, orientation, role)) {
    // FIXME: Implement me!
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

int CustomTableModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return items.size();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  if (items.empty())
    return 0;

  return db.getScheme().size();
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::TextAlignmentRole) {
    return Qt::AlignCenter;
  }

  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int column = index.column();

  string content;

  try {
    content = db.getProvider()->get(items[row], db.getSchemeField(column));
  } catch (exception const &e) {
    content = "";
  }

  if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole
      || role == Qt::UserRole)
    return QString::fromStdString(content);

  return QString("Undefined");
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
    return false;

  if (data(index, role) != value) {
    int row = index.row();
    int column = index.column();

    string content;

    if (column == 0)
      return false;
    try {
      string stdValue = value.toString().toStdString();
      db.getProvider()->set(items[row], db.getSchemeField(column), stdValue);
    } catch (exception const &e) {
      return false;
    }

    emit dataChanged(index, index, {role});
    return true;
  }
  return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void CustomTableModel::reset() {
  beginResetModel();

  items.clear();
  db.reset();

  endResetModel();
}

void CustomTableModel::clear() {
  beginResetModel();

  items.clear();
  db.clear();

  endResetModel();
}

void CustomTableModel::setFileName(const string &fileName) {
  db.setCurrentPath(fileName);
}

void CustomTableModel::readFromFile(const string &fileName) {
  setFileName(fileName);
  readFromFile();
}

void CustomTableModel::readFromFile() {
  clear();

  beginResetModel();

  db.read();
  db.parse(items);

  endResetModel();
}

void CustomTableModel::writeToFile(const string &fileName) {
  setFileName(fileName);
  writeToFile();
}

void CustomTableModel::writeToFile() {
  db.update(items);
  db.write();
}

void CustomTableModel::addEmptyRow() {
  beginResetModel();
  auto item = db.getProvider()->create();
  items.push_back(std::move(item));
  endResetModel();
}

void CustomTableModel::removeRow(size_t row) {
  if ((items.size() - 1) < row)
    return;

  beginResetModel();
  undoer.remember(items);
  items.erase(items.begin() + row);
  endResetModel();
}

shared_ptr<AbstractItem> &CustomTableModel::getItem(size_t row) {
  if (items.size() < row)
    throw runtime_error("Out of bounds on getItem(size_t) -> " + to_string(row));

  return items[row];
}

bool CustomTableModel::isEmpty() {
  return items.empty();
}

void CustomTableModel::rewind() {
  if (undoer.isEmpty())
    return;
  beginResetModel();
  items.clear();
  items = undoer.rewind();
  endResetModel();
}

DbFile const &CustomTableModel::getDb() const {
  return db;
}
