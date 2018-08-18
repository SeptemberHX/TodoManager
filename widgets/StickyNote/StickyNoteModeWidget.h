#ifndef STICKYNOTEMODEWIDGET_H
#define STICKYNOTEMODEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "../../core/StickyNote.h"
#include "../../data/DataCenter.h"
#include "StickyNoteTypeItemDelegate.h"
#include "StickyNoteWidget.h"

namespace Ui {
class StickyNoteModeWidget;
}

class StickyNoteModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StickyNoteModeWidget(QWidget *parent = 0);
    ~StickyNoteModeWidget();
    void appendStickyNote(const todo::StickyNote &stickyNote);
    void initStickyNoteWidget();

private:
    Ui::StickyNoteModeWidget *ui;
    StickyNoteTypeItemDelegate *itemDelegate;
    QStandardItemModel *itemModel;
    QMap<QString, StickyNoteWidget*> id2stickyNoteWidget;
    todo::DataCenter dataCenter;
    QMap<QString, QList<QStandardItem*>> id2ItemListMap;

    void dealWithNewStickyNote(const todo::StickyNote &stickyNote);
    todo::StickyNote collectStickyNoteByRow(int row);

private slots:
    void addButton_clicked();
    void stickyNoteWidget_moved(const QPoint &newPos);
    void item_changed(QStandardItem *item);
    void deleteButton_clicked();
};

#endif // STICKYNOTEMODEWIDGET_H
