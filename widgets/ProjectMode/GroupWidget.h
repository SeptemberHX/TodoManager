#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "GroupDetailWidget.h"

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
    QSplitter *mainSplitter;
};

#endif // GROUPWIDGET_H
