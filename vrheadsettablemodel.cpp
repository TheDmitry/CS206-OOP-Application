#include "vrheadsettablemodel.h"

VRHeadsetTableModel::VRHeadsetTableModel(QObject *parent)
  : QAbstractTableModel(parent)
{}

QVariant VRHeadsetTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        {
          return "Id";
        }
      case 1:
        {
          return "Name";
        }
      case 2:
        {
          return "Width";
        }
      case 3:
        {
          return "Height";
        }
      case 4:
        {
          return "Refresh Rate";
        }
      case 5:
        {
          return "Angles";
        }
      case 6:
        {
          return "Position";
        }
    }
  }
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

  // FIXME: Implement me!
}

int VRHeadsetTableModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  // FIXME: Implement me!
}

QVariant VRHeadsetTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role == Qt::DisplayRole)
    return QVariant();
}

bool VRHeadsetTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (data(index, role) != value) {
    // FIXME: Implement me!
    emit dataChanged(index, index, {role});
    return true;
  }
  return false;
}

Qt::ItemFlags VRHeadsetTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}
