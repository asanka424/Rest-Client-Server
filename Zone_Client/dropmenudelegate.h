#ifndef DROPMENUDELEGATE_H
#define DROPMENUDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QVector>

class DropMenuDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DropMenuDelegate(QVector<QString> &_items,QObject *parent = 0);
    ~DropMenuDelegate();

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;


public slots:
    void itemSelected(QString);

private:
    QVector<QString> Items;
};

#endif // DROPMENUDELEGATE_H
