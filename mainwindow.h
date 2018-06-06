#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "widgets/logwidget.h"
#include "widgets/todolistwidget.h"
#include "core/ItemDetail.h"
#include "data/DataCenter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void logAppend(const QString &str);

private:
    void initConfig();

    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout;
    QSplitter *splitter;
    LogWidget *logWidget;
    TodoListWidget *todoListWidget;

    // for notification
    QTimer *timer;
    qint64 interval;
    QList<todo::ItemDetail> targetItemDetails;
    QMutex notificationListMutex;
    todo::DataCenter dataCenter;

public slots:
    void change_statusBar_text(const QString &str);
    void update_notification_timer();
};

#endif // MAINWINDOW_H
