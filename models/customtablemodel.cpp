#include "models/customtablemodel.h"

CustomTableModel::CustomTableModel(QObject *parent)
  : QAbstractTableModel(parent) {}

CustomTableModel::~CustomTableModel() {
  items.clear();
  items.shrink_to_fit();

  db.reset();
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && !items.empty()) {
    if (section == 0)
      return "Id";

    return QString::fromStdString(db.getSchemeField(section - 1));
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

  return db.getScheme().size() + 1; // +1 due to "Id"
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

  if (column == 0) {
    content = to_string(row + 1);
  } else {
    try {
      content = db.getProvider()->get(items[row], db.getSchemeField(column - 1));
    } catch (std::exception const &e) {
      content = "";
    }
  }

  if (role == Qt::DisplayRole || role == Qt::EditRole)
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
      db.getProvider()->set(items[row], db.getSchemeField(column - 1), stdValue);
    } catch (std::exception const &e) {
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

  db.reset();
  items.clear();

  endResetModel();
}

void CustomTableModel::clear() {
  beginResetModel();

  db.clear();
  items.clear();

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
