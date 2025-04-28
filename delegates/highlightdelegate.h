#pragma once

#include <QStyledItemDelegate>
#include <QBrush>

class HighlightColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit HighlightColumnDelegate(QObject *parent = nullptr);

    void setHighlightColumn(int column);

protected:
  void paint(QPainter *painter,
             QStyleOptionViewItem const &option,
             QModelIndex const &index) const override;

private:
    int highlightColumn = -1;
};
