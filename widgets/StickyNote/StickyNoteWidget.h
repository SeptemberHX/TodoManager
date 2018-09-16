#ifndef STICKYNOTEWIDGET_H
#define STICKYNOTEWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDate>
#include <QPoint>
#include "../../config/StickyNoteConfig.h"
#include "../../data/DataCenter.h"

namespace Ui {
class StickyNoteWidget;
}

class StickyNoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StickyNoteWidget(const todo::StickyNote &stickyNote, QWidget *parent = nullptr);
    ~StickyNoteWidget();
    void setStickyNoteTitle(const QString &noteTitle);
    void loadStickyNote(const todo::StickyNote &stickyNote);
    void refresh_current_items();
    QString getStickyNoteId() const;

signals:
    void databaseModified();
    void widgetMoved(const QPoint &currPos);

private:
    explicit StickyNoteWidget(QWidget *parent = nullptr);

    QString stickyNoteId;
    Ui::StickyNoteWidget *ui;
    QStandardItemModel *itemModel;
    todo::DataCenter dataCenter;
    bool isClickedOnTitle;
    QPoint clickedOnTitlePoint;
    QPoint widgetPosWhenClickedOnTitle;
    bool isChangedByMySelf;
    todo::StickyNoteConfig config;

    void initWidgetStyle();
    QString getStyleSheet(const QColor &bgColor, const QColor &fontColor);
    void loadItemsByDate(const QDate &targetDate);

    /**
     * load this sticky note's config from config file
     */
    void loadConfig();

    /**
     * load tasks by config
     * @param config
     */
    void loadItemByConfig(const todo::StickyNoteConfig &config);

private slots:
    void list_item_changed(QStandardItem *item);
    void database_modified();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STICKYNOTEWIDGET_H
