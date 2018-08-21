//
// Created by septemberhx on 8/21/18.
//

#include "AppSystemTrayIcon.h"
#include <QIcon>
#include <QAction>
#include <QMenu>

AppSystemTrayIcon* AppSystemTrayIcon::instancePtr = nullptr;

void AppSystemTrayIcon::init(QWidget *parent) {
    this->trayIcon = new QSystemTrayIcon(QIcon(":/icons/tray.png"), parent);

    QMenu *trayMenu = new QMenu(parent);
    QAction *quitAction = new QAction("Quit", trayMenu);
    connect(quitAction, &QAction::triggered, this, &AppSystemTrayIcon::quitClicked);

    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(this->trayIcon, &QSystemTrayIcon::activated, this, &AppSystemTrayIcon::activated);
}

void AppSystemTrayIcon::quit_clicked() {
    emit quitClicked();
}

AppSystemTrayIcon::~AppSystemTrayIcon() {
    delete AppSystemTrayIcon::instancePtr;
}

AppSystemTrayIcon *AppSystemTrayIcon::getInstance() {
    if (AppSystemTrayIcon::instancePtr == nullptr) {
        AppSystemTrayIcon::instancePtr = new AppSystemTrayIcon();
    }

    return AppSystemTrayIcon::instancePtr;
}

void AppSystemTrayIcon::hide() {
    this->trayIcon->hide();
}

void AppSystemTrayIcon::showMessage(const QString &titleStr, const QString &bodyStr) {
    this->trayIcon->showMessage(titleStr, bodyStr);
}
