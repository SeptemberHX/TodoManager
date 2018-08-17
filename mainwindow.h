#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QButtonGroup>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "widgets/logwidget.h"
#include "widgets/todolistwidget.h"
#include "widgets/TagMode/TagModeWidget.h"
#include "widgets/CalendarMode/CalendarModeWidget.h"
#include "widgets/ProjectMode/ProjectModeWidget.h"
#include "widgets/StickyNote/StickyNoteWidget.h"
#include "widgets/StickyNote/StickyNoteModeWidget.h"
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
    TodoListWidget *inboxViewWidget;
    TagModeWidget *tagModeWidget;
    CalendarModeWidget *calendarModeWidget;
    ProjectModeWidget *projectModeWidget;
    StickyNoteModeWidget *stickyNoteModeWidget;

    // for notification
    QTimer *timer;
    qint64 interval;
    QList<todo::ItemDetail> targetItemDetails;
    QMutex notificationListMutex;
    todo::DataCenter dataCenter;

    // tray icon
    QSystemTrayIcon *trayIcon;

    // multi view mode support
    QButtonGroup *viewButtonGroup;
    int dailyMode;
    int inboxMode;
    int tagMode;
    int calendarMode;
    int projectMode;
    int stickyNoteMode;
    int currentMode;

private slots:
    void click_exit();
    void trayIcon_clicked();
    void modeBtn_clicked(QAbstractButton *button);
    void database_modified();
    void jump_to_specific_group(const QString &groupID);
    void jump_to_specific_tag(const QString &itemID);

public slots:
    void update_notification_timer();
    void item_clicked(const todo::ItemDetail &item);
    void targetDay_clicked(const QDate &targetDay);
};

#endif // MAINWINDOW_H
