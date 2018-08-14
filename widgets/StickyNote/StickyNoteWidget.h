#ifndef STICKYNOTEWIDGET_H
#define STICKYNOTEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDate>
#include "../../data/DataCenter.h"

namespace Ui {
class StickyNoteWidget;
}

class StickyNoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StickyNoteWidget(QWidget *parent = 0);
    ~StickyNoteWidget();
    void setStickyNoteTitle(const QString &noteTitle);

signals:
    void databaseModified();

private:
    Ui::StickyNoteWidget *ui;
    QStandardItemModel *itemModel;
    todo::DataCenter dataCenter;
    bool isClickedOnTitle;
    QPoint clickedOnTitlePoint;
    QPoint widgetPosWhenClickedOnTitle;

    void initWidgetStyle();
    QString getStyleSheet(const QColor &bgColor, const QColor &fontColor);
    void loadItemsByDate(const QDate &targetDate);

private slots:
    void list_item_changed(QStandardItem *item);
    void database_modified();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STICKYNOTEWIDGET_H
