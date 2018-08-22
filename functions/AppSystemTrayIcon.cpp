//
// Created by septemberhx on 8/21/18.
//

#include "AppSystemTrayIcon.h"
#include <QIcon>
#include <QDebug>

AppSystemTrayIcon* AppSystemTrayIcon::instancePtr = nullptr;

void AppSystemTrayIcon::init(QWidget *parent) {
    this->trayIcon = new QSystemTrayIcon(QIcon(":/icons/tray.png"), parent);

    this->trayMenu = new QMenu(parent);
    this->quitAction = new QAction("Quit", trayMenu);
    connect(quitAction, &QAction::triggered, this, &AppSystemTrayIcon::quitClicked);
    this->seprator = this->trayMenu->insertSeparator(this->quitAction);

    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(this->trayIcon, &QSystemTrayIcon::activated, this, &AppSystemTrayIcon::activated);
    this->update_menu();
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

void AppSystemTrayIcon::update_menu() {
    for (auto iter = this->menuPtr2IDMap.begin(); iter != this->menuPtr2IDMap.end(); ++iter) {
        this->trayMenu->removeAction(iter.key()->menuAction());
        delete iter.key();
    }
    this->menuPtr2IDMap.clear();

    auto todayTasks = this->dataCenter.selectItemDetailByDate(QDate::currentDate());
    foreach (auto const &task, todayTasks) {
        this->insertOneTaskToMenu(task);
    }
}

void AppSystemTrayIcon::insertOneTaskToMenu(const todo::ItemDetail &detail) {
    auto taskState = todo::TaskArchivingTimeRecorder::getInstance()->getTaskArchivingState(detail.getId());

    QMenu *taskMenu = new QMenu(detail.getTitle(), this->trayMenu);
    taskMenu->setIcon(this->getIconByState(taskState));

    QAction *startAction = new QAction("Start", taskMenu);
    connect(startAction, &QAction::triggered, this, &AppSystemTrayIcon::start_clicked);
    if (taskState != todo::TaskArchivingState::NOT_START) {
        startAction->setVisible(false);
    }

    QAction *pauseAction = new QAction("Pause", taskMenu);
    connect(pauseAction, &QAction::triggered, this, &AppSystemTrayIcon::pause_clicked);
    if (taskState != todo::TaskArchivingState::DOING) {
        pauseAction->setVisible(false);
    }

    QAction *resumeAction = new QAction("Resume", taskMenu);
    connect(resumeAction, &QAction::triggered, this, &AppSystemTrayIcon::resume_clicked);
    if (taskState != todo::TaskArchivingState::PAUSE && taskState != todo::TaskArchivingState::FINISH) {
        resumeAction->setVisible(false);
    }

    QAction *stopAction = new QAction("Finish", taskMenu);
    connect(stopAction, &QAction::triggered, this, &AppSystemTrayIcon::finish_clicked);

    taskMenu->addActions({startAction, pauseAction, resumeAction, stopAction});
    this->trayMenu->insertMenu(this->seprator, taskMenu);
    this->menuPtr2IDMap[taskMenu] = detail.getId();
}

void AppSystemTrayIcon::start_clicked() {
    emit archivingOperated(this->menuPtr2IDMap[dynamic_cast<QMenu*>(sender()->parent())],
            todo::TaskArchivingOperation::OPERATION_START);
}

void AppSystemTrayIcon::pause_clicked() {
    emit archivingOperated(this->menuPtr2IDMap[dynamic_cast<QMenu*>(sender()->parent())],
            todo::TaskArchivingOperation::OPERATION_PAUSE);
}

void AppSystemTrayIcon::resume_clicked() {
    emit archivingOperated(this->menuPtr2IDMap[dynamic_cast<QMenu*>(sender()->parent())],
            todo::TaskArchivingOperation::OPERATION_RESUME);
}

void AppSystemTrayIcon::finish_clicked() {
    emit archivingOperated(this->menuPtr2IDMap[dynamic_cast<QMenu*>(sender()->parent())],
            todo::TaskArchivingOperation::OPERATION_FINISH);
}

QIcon AppSystemTrayIcon::getIconByState(const todo::TaskArchivingState &state) {
    QIcon result = QIcon::fromTheme("player_start");
    switch (state) {
        case todo::TaskArchivingState::NOT_START:
            result = QIcon::fromTheme("player_start");
            break;
        case todo::TaskArchivingState::DOING:
            result = QIcon::fromTheme("player_record");
            break;
        case todo::TaskArchivingState::PAUSE:
            result = QIcon::fromTheme("player_pause");
            break;
        case todo::TaskArchivingState::FINISH:
            result = QIcon::fromTheme("player_stop");
            break;
        default:
            break;
    }
    return result;
}
