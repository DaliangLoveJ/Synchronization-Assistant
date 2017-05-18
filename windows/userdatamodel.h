#ifndef USERDATAMODEL_H
#define USERDATAMODEL_H
#include "functions/common.h"
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QModelIndex>
#include <QEvent>
#include <QAbstractItemModel>
#include <QHeaderView>

class TableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
    TableModel(QObject *parent = 0);
    ~TableModel();
private:
    QList<FileRecord> m_recordList;

public:
    void updateData(QList<FileRecord> recordList);
    int rowCount(const QModelIndex &index) const;
    int rowNumber() const;
    int columnCount(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CheckBoxDelegate(QObject *parent = 0);
    ~CheckBoxDelegate();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // USERDATAMODEL_H

