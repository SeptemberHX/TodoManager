#ifndef NAVIGATIONBARWIDGET_H
#define NAVIGATIONBARWIDGET_H

#include <QWidget>

namespace Ui {
class NavigationBarWidget;
}

class NavigationBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBarWidget(QWidget *parent = 0);
    ~NavigationBarWidget();

private:
    Ui::NavigationBarWidget *ui;
};

#endif // NAVIGATIONBARWIDGET_H
