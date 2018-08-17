//
// Created by septemberhx on 8/17/18.
//

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAbstractItemModel>
#include "StickyNoteTypeItemDelegate.h"
#include "../../core/EnumTypes.h"


StickyNoteTypeItemDelegate::StickyNoteTypeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

QWidget *StickyNoteTypeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const {
    QString str = index.model()->data(index, Qt::EditRole).toString();

    // must set parent here. Or combobox will not show in the table cell
    QComboBox *typeComboBox = new QComboBox(parent);
    typeComboBox->setFixedHeight(option.rect.height());
    this->initComboBox(typeComboBox, str);
    return typeComboBox;
}

void StickyNoteTypeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox != nullptr) {
        QString str = index.model()->data(index, Qt::EditRole).toString();
        comboBox->setCurrentIndex(comboBox->findText(str));
    }
}

void StickyNoteTypeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox != nullptr) {
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
}

void StickyNoteTypeItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

void
StickyNoteTypeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionComboBox box;
    box.rect = option.rect;
    box.currentText = index.model()->data(index, Qt::EditRole).toString();

    QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &box, painter);
    QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &box, painter);
}

void StickyNoteTypeItemDelegate::initComboBox(QComboBox *comboBox, const QString &currentStr) const {
    comboBox->clear();
    comboBox->addItem(todo::StickyNoteType::DAILY);
    comboBox->addItem(todo::StickyNoteType::TAG);
    comboBox->addItem(todo::StickyNoteType::PROJECT);
    comboBox->addItem(todo::StickyNoteType::CUSTOM);
    comboBox->setCurrentText(currentStr);
}
