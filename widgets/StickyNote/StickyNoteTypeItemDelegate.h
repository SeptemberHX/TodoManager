//
// Created by septemberhx on 8/17/18.
//

#ifndef TODOMANAGER_STICKYNOTEITEMDELEGATE_H
#define TODOMANAGER_STICKYNOTEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

class StickyNoteTypeItemDelegate: public QStyledItemDelegate {
public:
    StickyNoteTypeItemDelegate(QObject *parent = 0);

    void
    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    void initComboBox(QComboBox *comboBox, const QString &currentStr) const;
};


#endif //TODOMANAGER_STICKYNOTEITEMDELEGATE_H
