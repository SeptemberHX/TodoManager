#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>
#include "../../core/ItemGroup.h"

namespace Ui {
class GroupDetailWidget;
}

class GroupDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupDetailWidget(QWidget *parent = 0);
    ~GroupDetailWidget();
    void loadItemGroup(const todo::ItemGroup &itemGroup);
private:
    Ui::GroupDetailWidget *ui;
};

#endif // GROUPDETAILWIDGET_H
