#ifndef PROJECTMODEWIDGET_H
#define PROJECTMODEWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectModeWidget;
}

class ProjectModeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectModeWidget(QWidget *parent = 0);
    ~ProjectModeWidget();

private:
    Ui::ProjectModeWidget *ui;
};

#endif // PROJECTMODEWIDGET_H
