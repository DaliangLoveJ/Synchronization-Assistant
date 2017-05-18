#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H
#include <QStyledItemDelegate>

class ComboBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
    ComboBoxDelegate(QObject * parent=0);
    virtual ~ComboBoxDelegate(){}
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void deleteItem(const QModelIndex &index);

};
#endif // COMBOBOXDELEGATE_H

