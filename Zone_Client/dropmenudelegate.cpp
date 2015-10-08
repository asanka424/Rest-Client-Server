#include "dropmenudelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QFile>

#include <iostream>

DropMenuDelegate::DropMenuDelegate(QVector<QString> &_items,QObject *parent)
:QStyledItemDelegate(parent),Items(_items)
{
}


QWidget *DropMenuDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QComboBox* editor = new QComboBox(parent);
  QFile File(":Data/resources/styles.css");
   if (File.open(QFile::ReadOnly))
   {
       QString StyleSheet = QLatin1String(File.readAll());

       editor->setStyleSheet(StyleSheet);
   }

  connect(editor,SIGNAL(currentTextChanged(QString)),this,SLOT(itemSelected(QString)),Qt::QueuedConnection);
  for(int i = 0; i < Items.size(); ++i)
    {
    editor->addItem(Items[i]);
    }
  return editor;
}

void DropMenuDelegate::itemSelected(QString val)
{
    //emit commitData(qobject_cast<QComboBox*>(sender()));
    closeEditor(qobject_cast<QComboBox*>(sender()));
}

void DropMenuDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
          // get the index of the text in the combobox that matches the current value of the itenm
          QString currentText = index.data(Qt::EditRole).toString();
          int cbIndex = cb->findText(currentText);
          // if it is valid, adjust the combobox
          if (cbIndex >= 0)
              cb->setCurrentIndex(cbIndex);
       } else {
           QStyledItemDelegate::setEditorData(editor, index);
       }
}

void DropMenuDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
    {
            // save the current text of the combo box as the current value of the item
            model->setData(index, cb->currentText(), Qt::EditRole);\
            qDebug() << "set data to " << cb->currentText();
    }
        else
            QStyledItemDelegate::setModelData(editor, model, index);
}

void DropMenuDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void DropMenuDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyleOptionViewItemV4 myOption = option;
  QString text = Items[index.row()];

  myOption.text = text;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

DropMenuDelegate::~DropMenuDelegate()
{

}

