//
// Created by septemberhx on 8/21/18.
//

#ifndef TODOMANAGER_APPSYSTEMTRAYICON_H
#define TODOMANAGER_APPSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QString>
#include <QObject>
#include <QAction>
#include <QMenu>
#include "../data/DataCenter.h"
#include "TaskArchivingTimeRecorder.h"

class AppSystemTrayIcon : public QObject {
    Q_OBJECT
public:
    static AppSystemTrayIcon *getInstance();
    void init(QWidget *parent = nullptr);
    ~AppSystemTrayIcon();
    void hide();
    void showMessage(const QString &titleStr, const QString &bodyStr);

signals:
    void activated();
    void quitClicked();
    void archivingOperated(const QString &itemID, const todo::TaskArchivingOperation &operation);

public slots:
    void update_menu();

private:
    static AppSystemTrayIcon *instancePtr;

    AppSystemTrayIcon() = default;
    QSystemTrayIcon *trayIcon;
    todo::DataCenter dataCenter;
    QMenu *trayMenu;
    QAction *quitAction;
    QAction *seprator;
    QMap<QMenu*, QString> menuPtr2IDMap;

    void insertOneTaskToMenu(const todo::ItemDetail &detail);
    QIcon getIconByState(const todo::TaskArchivingState &state);

private slots:
    void quit_clicked();
    void start_clicked();
    void pause_clicked();
    void resume_clicked();
    void finish_clicked();
};


#endif //TODOMANAGER_APPSYSTEMTRAYICON_H
