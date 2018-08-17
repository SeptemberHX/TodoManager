#ifndef STICKYNOTEMODEWIDGET_H
#define STICKYNOTEMODEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "../../core/StickyNote.h"
#include "StickyNoteTypeItemDelegate.h"

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

private:
    Ui::StickyNoteModeWidget *ui;
    StickyNoteTypeItemDelegate *itemDelegate;
    QStandardItemModel *itemModel;
};

#endif // STICKYNOTEMODEWIDGET_H
