#include "models/customtablemodel.h"
#include "qcoreapplication.h"
#include "qnamespace.h"
#include <QIODevice>
#include <array>

using namespace std;

CustomTableModel::CustomTableModel(QObject *parent)
    : QAbstractTableModel(parent), items{} {}

CustomTableModel::~CustomTableModel() {
  items.clear();
  items.shrink_to_fit();

  db.reset();
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole &&
      !items.empty()) {
    return QString::fromStdString(db.getSchemeField(section));
  }

  if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
    return QString::number(section + 1);
  }

  return QVariant();
}

bool CustomTableModel::setHeaderData(int section, Qt::Orientation orientation,
                                     QVariant const &value, int role) {
  if (value != headerData(section, orientation, role)) {
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

int CustomTableModel::rowCount(QModelIndex const &parent) const {
  if (parent.isValid())
    return 0;

  return items.size();
}

int CustomTableModel::columnCount(QModelIndex const &parent) const {
  if (parent.isValid())
    return 0;

  if (items.empty())
    return 0;

  return db.getScheme().size();
}

QVariant CustomTableModel::data(QModelIndex const &index, int role) const {
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

  if (role == Qt::DisplayRole || role == Qt::EditRole ||
      role == Qt::ToolTipRole || role == Qt::UserRole)
    return QString::fromStdString(content);

  return QString("Undefined");
}

bool CustomTableModel::setData(QModelIndex const &index, QVariant const &value,
                               int role) {
  if (!index.isValid())
    return false;

  if (data(index, role) != value) {
    int row = index.row();
    int column = index.column();

    if (!undoer.remember(items, [&value, row, column, this]() {
          string stdValue = value.toString().toStdString();
          db.getProvider()->set(items[row], db.getSchemeField(column),
                                stdValue);
        }))
      return false;

    emit dataChanged(index, index, {role});
    return true;
  }
  return false;
}

Qt::ItemFlags CustomTableModel::flags(QModelIndex const &index) const {
  if (!index.isValid())
    return Qt::ItemIsDropEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
         Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool CustomTableModel::insertRows(int row, int count,
                                  QModelIndex const &parent) {
  if (parent.isValid())
    return false;

  if (row < 0 || row > rowCount() || count <= 0)
    return false;

  beginInsertRows(parent, row, row + count - 1);

  undoer.remember(items, [this, row, count]() {
    auto item = db.getProvider()->create();
    items.insert(items.begin() + row, count, item);
  });

  endInsertRows();
  return true;
}

bool CustomTableModel::removeRows(int row, int count,
                                  const QModelIndex &parent) {
  if (parent.isValid())
    return false;

  if (row < 0 || (row + count) > rowCount() || count <= 0)
    return false;

  beginRemoveRows(parent, row, row + count - 1);

  undoer.remember(items, [this, row, count]() {
    items.erase(items.begin() + row, items.begin() + row + count);
  });

  endRemoveRows();
  return true;
}

bool CustomTableModel::insertRow(int row, QModelIndex const &parent) {
  return insertRows(row, 1, parent);
}

bool CustomTableModel::removeRow(int row, QModelIndex const &parent) {
  return removeRows(row, 1, parent);
}

void CustomTableModel::reset() {
  beginResetModel();

  items.clear();
  db.reset();

  endResetModel();

  emit dbReset();
}

void CustomTableModel::clear() {
  beginResetModel();

  items.clear();
  db.clear();

  endResetModel();
  emit dbClear();
}

void CustomTableModel::setFileName(string const &fileName) {
  db.setCurrentPath(fileName);
}

void CustomTableModel::readFromFile(string const &fileName) {
  setFileName(fileName);
  readFromFile();
}

void CustomTableModel::readFromFile() {
  clear();

  beginResetModel();

  db.read();
  db.parse(items);

  endResetModel();

  emit fileLoaded(db.getCurrentPath());
}

void CustomTableModel::writeToFile(string const &fileName) {
  setFileName(fileName);
  writeToFile();
}

void CustomTableModel::writeToFile() {
  db.update(items);
  db.write();
}

CustomTableModel::ItemType &CustomTableModel::getItem(size_t row) {
  if (items.size() < row)
    throw runtime_error("Out of bounds on getItem(size_t) -> " +
                        to_string(row));

  return items[row];
}

CustomTableModel::ContainerType &CustomTableModel::getItems() { return items; }

bool CustomTableModel::isEmpty() { return items.empty(); }

void CustomTableModel::rewind() {
  if (undoer.isEmpty())
    return;
  beginResetModel();
  items.clear();
  items = undoer.rewind();
  endResetModel();
}

DbFile const &CustomTableModel::getDb() const { return db; }

// Drag&Drop functionality
QStringList CustomTableModel::mimeTypes() const {
  return {
      "application/vnd.customtablemodel.data", // Для внутреннего перемещения
      "text/plain" // Для экспорта в текстовые редакторы
  };
}

QMimeData *CustomTableModel::mimeData(QModelIndexList const &indexes) const {
  (void)indexes;

  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  stream << QCoreApplication::applicationPid(); // application pid

  stream << QString::fromStdString(
      DbFile::SUPPORTED_VERSION); // supported version of db file in
                                  // current implementation

  stream << QString::fromStdString(
      db.getProvider()->getName()); // current used provider

  stream << static_cast<int>(items.size()); // amount of rows in QTableModel
  stream << static_cast<int>(db.getScheme().size());

  for (size_t i = 0; i < db.getScheme().size(); i++) {
    stream << QString::fromStdString(db.getSchemeField(i));
  }

  for (size_t row = 0; row < items.size(); row++) {
    for (int col = 0; col < columnCount(); col++) {
      stream << QString::fromStdString(
          db.getProvider()->get(items[row], db.getSchemeField(col)));
    }
  }
  mimeData->setData("application/vnd.customtablemodel.data", encodedData);

  // plain text stream
  QString plainText;
  plainText += "Item: " + db.getProvider()->getItemName() + "\n";
  plainText += "Provider: " + db.getProvider()->getName() + "\n";
  plainText += "Count: " + QString::fromStdString(to_string(rowCount())) + "\n";
  plainText += "Scheme: \n";
  for (size_t col = 0; col < db.getScheme().size(); col++) {
    plainText += db.getSchemeField(col) + "\t";
  }

  plainText += "\n";

  for (size_t row = 0; row < items.size(); row++) {
    if (row >= 0 && row < items.size()) {
      for (int col = 0; col < columnCount(); ++col) {
        if (col > 0)
          plainText += "\t";
        plainText += data(index(row, col), Qt::DisplayRole).toString();
      }
      plainText += "\n";
    }
  }
  mimeData->setText(plainText);

  return mimeData;
}

// This happes whenever application is expected to do something with dropped
// content
bool CustomTableModel::dropMimeData(const QMimeData *data,
                                    Qt::DropAction action, int row, int column,
                                    const QModelIndex &parent) {
  (void)parent;
  (void)column;
  (void)row;

  if (!data->hasFormat("application/vnd.customtablemodel.data"))
    return false;

  if (action == Qt::IgnoreAction)
    return true;

  QByteArray encodedData = data->data("application/vnd.customtablemodel.data");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  // pid check
  qint64 applicationPid;
  stream >> applicationPid;
  if (applicationPid == QCoreApplication::applicationPid())
    return false;

  // Supported DbFile version check
  QString SUPPORTED_VERSION;
  stream >> SUPPORTED_VERSION;
  if (DbFile::SUPPORTED_VERSION != SUPPORTED_VERSION)
    return false;

  // Provider name check&set
  QString providerName;
  stream >> providerName;
  if (!db.getProviders().contains(providerName.toStdString()))
    return false;
  db.setProvider(providerName.toStdString());

  // num of rows check&set
  int numRows = 0;
  stream >> numRows;
  if (numRows <= 0)
    return false;

  // num of scheme fields check&set
  int numSchemeFields = 0;
  stream >> numSchemeFields;
  if (numSchemeFields <= 0)
    return false;

  map<string, size_t> scheme;
  vector<string> order;
  QString fieldName;
  for (int i = 0; i < numSchemeFields; i++) {
    stream >> fieldName;
    string str = fieldName.toStdString();

    scheme[str] = i;
    order.push_back(str);
  }
  db.setScheme(scheme);

  undoer.remember(items);

  beginResetModel();
  items.clear();
  items.shrink_to_fit();

  for (int i = 0; i < numRows; ++i) {
    ItemType item = db.getProvider()->create();
    for (size_t key = 0; key < order.size(); key++) {
      QString value;
      stream >> value;
      db.getProvider()->set(item, db.getSchemeField(key), value.toStdString());
    }
    items.push_back(item);
  }
  endResetModel();
  return true;
}

Qt::DropActions CustomTableModel::supportedDropActions() const {
  return Qt::CopyAction;
}
