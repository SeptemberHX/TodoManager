#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QStandardItemModel>
#include "GroupDetailWidget.h"
#include "../itemlistwidget.h"
#include "../itemdetailwidget.h"
#include "../../core/ItemAndGroupWrapper.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupWidget(QWidget *parent = 0);
    ~GroupWidget();

private:
    Ui::GroupWidget *ui;
    GroupDetailWidget *groupDetailWidget;
    ItemDetailWidget *itemDetailWidget;
    ItemListWidget *listWidget;
    QSplitter *mainSplitter;

    QMap<QString, todo::ItemAndGroupWrapper> itemMap;

    void loadItems(const QList<todo::ItemAndGroupWrapper> &itemList);
private slots:

    void selected_item_changed(const QString &itemID);
};

#endif // GROUPWIDGET_H
