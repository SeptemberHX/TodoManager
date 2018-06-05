#ifndef FETCHCONFIGFILEPATHWIDGET_H
#define FETCHCONFIGFILEPATHWIDGET_H

#include <QWidget>

namespace Ui {
class FetchConfigFilePathWidget;
}

class FetchConfigFilePathWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FetchConfigFilePathWidget(QWidget *parent = 0);
    ~FetchConfigFilePathWidget();

private:
    Ui::FetchConfigFilePathWidget *ui;
};

#endif // FETCHCONFIGFILEPATHWIDGET_H
