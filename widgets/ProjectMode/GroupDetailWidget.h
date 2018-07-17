#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>

namespace Ui {
class GroupDetailWidget;
}

class GroupDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupDetailWidget(QWidget *parent = 0);
    ~GroupDetailWidget();

private:
    Ui::GroupDetailWidget *ui;
};

#endif // GROUPDETAILWIDGET_H
