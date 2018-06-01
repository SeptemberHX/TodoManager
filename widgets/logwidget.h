#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>

namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = 0);
    ~LogWidget();

    void append(const QString &str);

private:
    Ui::LogWidget *ui;
};

#endif // LOGWIDGET_H
