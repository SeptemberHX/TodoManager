#ifndef TAGMODEWIDGET_H
#define TAGMODEWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "todolistwidget.h"

namespace Ui {
class TagModeWidget;
}

class TagModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagModeWidget(QWidget *parent = 0);
    ~TagModeWidget();

private:
    Ui::TagModeWidget *ui;

    TodoListWidget *todoListWidget;
    QSplitter *mainSplitter;
};

#endif // TAGMODEWIDGET_H
