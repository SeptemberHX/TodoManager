#ifndef NAVIGATIONBARWIDGET_H
#define NAVIGATIONBARWIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class NavigationBarWidget;
}

class NavigationBarWidget : public QWidget
{
    Q_OBJECT

public:
    static const QString ROOT;
    explicit NavigationBarWidget(QWidget *parent = 0);
    ~NavigationBarWidget();
    void append(const QString &id, const QString &name);
    void clear();

signals:
    void jumpTo(const QList<QString> &pathList);

private:
    Ui::NavigationBarWidget *ui;
    QList<QWidget*> widgetPtrList;  // store button, label by order
    QMap<QString, QString> buttonObjectName2ID;

    QList<QString> getPathIDList();  // get the id path chain
    void appendRoot();

private slots:
    void toolButton_clicked();
};

#endif // NAVIGATIONBARWIDGET_H
