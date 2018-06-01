#ifndef ITEMLISTITEMWIDGET_H
#define ITEMLISTITEMWIDGET_H

#include <QWidget>
#include <QColor>
#include <memory>
#include "../core/ItemDetail.h"

namespace Ui {
class ItemListItemWidget;
}

class ItemListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemListItemWidget(QWidget *parent = 0);
    ~ItemListItemWidget();

    void setTagColor(const QColor &color);
    void loadItemDetail(const todo::ItemDetail &item);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ItemListItemWidget *ui;
    std::shared_ptr<todo::ItemDetail> itemDetailPtr;
    void reloadItemDetail();
    void setDoneLabelColor(bool isDone);
};

#endif // ITEMLISTITEMWIDGET_H
