#include "delegates/highlightdelegate.h"
#include <QPainter>

using namespace std;

HighlightColumnDelegate::HighlightColumnDelegate(QObject *parent)
  : QStyledItemDelegate(parent) {}

void HighlightColumnDelegate::setHighlightColumn(int column)
{
  highlightColumn = column;
}

void HighlightColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
  QStyleOptionViewItem opt{option};

  if (index.column() == highlightColumn) {
    opt.font.setBold(true);
  }

  QStyledItemDelegate::paint(painter, opt, index);
}

