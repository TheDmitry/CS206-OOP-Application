#include "vrheadsettablemodel.h"
#include "vector3.h"
#include <iostream>
#include <variant>

VRHeadsetTableModel::VRHeadsetTableModel(QObject *parent)
  : QAbstractTableModel(parent)
  , columnsNum(7) {}

QVariant VRHeadsetTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return "Id";
      case 1:
        return "Name";
      case 2:
        return "Width";
      case 3:
        return "Height";
      case 4:
        return "Refresh Rate";
      case 5:
        return "Angles";
      case 6:
        return "Position";
      default:
        throw "Something bad happened. Definetly bug";
    }
  }

  if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
    return QString::number(section + 1);
  }

  return QVariant();
}

bool VRHeadsetTableModel::setHeaderData(int section,
                                        Qt::Orientation orientation,
                                        const QVariant &value,
                                        int role)
{
  if (value != headerData(section, orientation, role)) {
    // FIXME: Implement me!
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

int VRHeadsetTableModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return headsets.size();
}

int VRHeadsetTableModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return columnsNum;
}

QVariant VRHeadsetTableModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::TextAlignmentRole) {
    return Qt::AlignCenter;
  }

  if (!index.isValid())
    return QVariant();

  int row = index.row();
  int column = index.column();
  auto &item = headsets[row];

  string content;
  switch (column) {
    case 0: {
      content = to_string(row + 1);
    } break;
    case 1: {
      content = item.getModelName();
    } break;
    case 2: {
      content = to_string(item.getWidth());
    } break;
    case 3: {
      content = to_string(item.getHeight());
    } break;
    case 4: {
      content = to_string(item.getRefreshRate());
    } break;
    case 5: {
      content = Vector3::vectorToString(item.getAngles());
    } break;
    case 6: {
      content = Vector3::vectorToString(item.getPosition());
    } break;
    default: {
      content = "";
    }
  }

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    return QString::fromStdString(content);

  return QString("Undefined");
}

bool VRHeadsetTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
    return false;

  if (data(index, role) != value) {
    int row = index.row();
    int column = index.column();
    auto &item = headsets[row];

    string content;
    switch (column) {
      case 0: {       // Id
        return false; // Id is INDEX in container so non-editable
      }
      case 1: { // modelName
        try {
          string stdValue = value.toString().toStdString();
          for (auto &i : headsets) {
            if (i.getModelName() == stdValue)
              return false; // modelName must be unique in whole container
          }

          item.setModelName(stdValue);
        } catch (exception const &e) {
          return false;
        }
      } break;
      case 2: { // width
        try {
          item.setWidth(value.toInt());
        } catch (exception const &e) {
          return false;
        }
      } break;
      case 3: { // height
        try {
          item.setHeight(value.toInt());
        } catch (exception const &e) {
          return false;
        }
      } break;
      case 4: { // refR
        try {
          item.setRefreshRate(value.toFloat());
        } catch (exception const &e) {
          return false;
        }
      } break;
      case 5: { // v3(angles)
        try {
          item.setAngles(Vector3::vectorFromString(value.toString().toStdString()));
        } catch (exception const &e) {
          return false;
        }
      } break;
      case 6: { // v3(position)
        try {
          item.setPosition(Vector3::vectorFromString(value.toString().toStdString()));
        } catch (exception const &e) {
          return false;
        }
      } break;
      default: {
        return false;
      }
    }

    emit dataChanged(index, index, {role});
    return true;
  }
  return false;
}

Qt::ItemFlags VRHeadsetTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void VRHeadsetTableModel::reset() {
  beginResetModel();

  db.reset();
  headsets.clear();

  endResetModel();
}

void VRHeadsetTableModel::clear() {
  beginResetModel();

  db.clear();
  headsets.clear();

  endResetModel();
}

void VRHeadsetTableModel::setFileName(const string fileName) {
  db.setCurrentPath(fileName);
}

void VRHeadsetTableModel::readFromFile(const string fileName) {
  setFileName(fileName);
  readFromFile();
}

void VRHeadsetTableModel::readFromFile() {
  clear();

  beginResetModel();

  db.read();
  db.parse(headsets);

  endResetModel();
}

void VRHeadsetTableModel::writeToFile(const string fileName) {
  setFileName(fileName);
  writeToFile();
}

void VRHeadsetTableModel::writeToFile() {
  db.update(headsets);
  db.write();
}
